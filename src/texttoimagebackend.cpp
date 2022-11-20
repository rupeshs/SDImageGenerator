/*
    SDImageGenerator, Text to image generation AI app
    Copyright(C) 2022 Rupesh Sreeraman
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "texttoimagebackend.h"


TextToImageBackend::TextToImageBackend(QObject *parent)
    : QObject{parent}
{
    isProcessing = false;

    m_options = new DiffusionOptions();
    diffusionEnv = new DiffusionEnvironment(parent);
    diffusionEnv->getEnvironment();

    appSettings = new Settings(parent, m_options,diffusionEnv);

    stableDiffusion = new DiffusionProcess(parent,diffusionEnv);
    connect(stableDiffusion, SIGNAL(generatingImages()), this, SLOT(generatingImages()));
    connect(stableDiffusion, SIGNAL(imagesGenerated()), this, SLOT(imagesGenerated()));
    connect(stableDiffusion, SIGNAL(diffusionFinished()), this, SLOT(stableDiffusionFinished()));
    connect(stableDiffusion, SIGNAL(gotConsoleLog(QString)), this, SLOT(updateStatusMessage(QString)));
    connect(stableDiffusion, SIGNAL(cudaMemoryError()), this, SLOT(cudaMemoryError()));
    connect(stableDiffusion, SIGNAL(stopped()), this, SLOT(diffusionCancelled()));
    connect(stableDiffusion, SIGNAL(modelLoaded(bool)), this, SLOT(stableDiffusionModelLoaded(bool)));

    deafultAssetsPath = Utils::pathAppend(qApp->applicationDirPath(),"default");
    samplesPath = Utils::localPathToUrl(deafultAssetsPath);

    deviceDetector = new DeviceDetector(this,diffusionEnv);
    connect(deviceDetector, SIGNAL(gotDeviceInfo(QString)), this, SLOT(updateDeviceInfo(QString)));
    deviceDetector->detect();

    m_envStatus = new DiffusionEnvironmentStatus();

    modelDownloader = nullptr;
    pythonEnvInstaller = nullptr;
    isCancelled = false;

    modelsFinder = new ModelsFinder(this,diffusionEnv);
    connect(modelsFinder, SIGNAL(gotModelsList(QStringList)), this, SLOT(gotModelsList(QStringList)));
    modelsFinder->find();

    isStableDiffusionModelLoading = false;
    isModelLoaded = false;
    clipboard = QGuiApplication::clipboard();

}

void TextToImageBackend::generateImage(bool isVariation)
{
    isCancelled = false;

    if (!m_options->acceptedTerms()) {
        showTermsWindow();
        return;
    }

    if (stableDiffusion->getUseTiConcept() != m_options->useTextualInversion()) {
         qDebug()<<"TI Status : "<<m_options->useTextualInversion();
        stableDiffusion->stopProcess();
    }


    if (m_options->useTextualInversion()){
        if (!stableDiffusion->getCurTiConcept().isEmpty() && stableDiffusion->getCurTiConcept() != m_options->tiConceptStyle()) {
            qDebug()<<"Changing TI concept to "<< m_options->tiConceptStyle();
            stableDiffusion->stopProcess();
        }
    }

    if(isVariation) {
        if (!stableDiffusion->isDreamRunning()){
          showErrorDlg(tr("To generate image variations,please generate images first."));
          return;
        }
    }

    if (!Utils::checkPathExists(m_options->saveDir())) {
        showErrorDlg(tr("Please choose a output directory from settings tab."));
        return;
    }

    if (m_options->imageToImage()) {

        if(!isValidInitImage()) {
            showErrorDlg(tr("File not found,please choose a valid initial image"));
            return;
        }

        if (m_options->useMaskImage()) {
            if(!Utils::checkPathExists(m_options->maskImagePath())) {
                showErrorDlg(tr("File not found,please choose a valid mask image"));
                return;
            }
        }
    }

    if (m_options->faceRestoration()) {
        qDebug()<<m_options->faceRestorationMethod();
        if (m_options->faceRestorationMethod() == "GFPGAN") {
            if(!envValidator->validateGfpGanModel()) {
                showErrorDlg(tr("To use face restoration, please download GFPGAN model from downloads tab."));
                return;
            }
        } else {
            if(!envValidator->validateCodeFormerModel()) {
                showErrorDlg(tr("To use face restoration, please download Code Former model from downloads tab."));
                return;
            }
        }
    }

    if (m_options->prompt().isEmpty()) {
        showErrorDlg(tr("Please provide a prompt text."));
        return;
    }

    if (stableDiffusion->getStatus() == StableDiffusionStatus::NotStarted)
        updateStatusMessage(tr("Loading model,please wait..."));
    else
        updateStatusMessage(tr("Starting image generation..."));

    qInfo()<<"Prompt : "<< m_options->prompt().trimmed();
    qInfo()<<"Scale : "<< m_options->scale();
    qInfo()<<"Image width :"<< m_options->imageWidth();
    qInfo()<<"Image height :"<< m_options->imageHeight();
    qInfo()<<"Number of Images to generate :"<< m_options->numberOfImages();
    qInfo()<<"DDIM steps :"<< m_options->ddimSteps();
    qInfo()<<"Sampler :"<< m_options->sampler();
    qInfo()<<"Seed :"<< m_options->seed();
    qInfo()<<"Save dir :"<< m_options->saveDir();
    isProcessing = true;
    stableDiffusion->generateImages(m_options,isVariation);
    samplesPath = Utils::localPathToUrl(deafultAssetsPath);
    emit samplesPathChanged();
    emit isProcessingChanged();
}

void TextToImageBackend::stopProcessing()
{
    updateStatusMessage(tr("Cancelling please wait..."));
    stableDiffusion->stopProcess();
    isProcessing = false;
    isModelLoaded = false;
    isCancelled = true;
    emit isCancelledChanged();
    emit isProcessingChanged();
}

void TextToImageBackend::showErrorDlg(const QString &error)
{
    errorMsg = error;
    emit gotErrorMessage();
    emit showMessageBox();
}

void TextToImageBackend::saveSettings()
{
    qDebug()<<"Save StableDiffusion settings : "<<QString::number(m_options->metaObject()->propertyCount());
    appSettings->save();
}

void TextToImageBackend::loadSettings()
{
    qInfo()<<"Loading app settings";
    appSettings->load();

    if (!m_options->acceptedTerms()){
        showTermsWindow();
        return;
    }

    qInfo()<<"Textual inversion directory : "<<m_options->tiConceptDirectory();

    Utils::ensurePath(m_options->saveDir());

    loadTiConceptsNamesFromFolder(m_options->tiConceptDirectory());

    envValidator = new DiffusionEnvValidator(this,diffusionEnv);
    connect(envValidator, SIGNAL(environmentCurrentStatus(bool,bool)), this, SLOT(environmentCurrentStatus(bool,bool)));

    if (QString::compare(m_options->appPath(),qApp->applicationDirPath(),Qt::CaseInsensitive) != 0 ){
        envValidator->ValidatePythonPackages();
        emit setupInstallerUi(false);
    }
    else{
        bool isStableDiffusionModelReady = true;
        handleModelStatus(isStableDiffusionModelReady);
        qDebug()<<"Custom models enabled(Advanced mode)";
        initAppControls(true,isStableDiffusionModelReady);
    }
}

void TextToImageBackend::resetSettings()
{
    appSettings->reset();
    emit initControls(m_options,m_envStatus);
}

void TextToImageBackend::stableDiffusionFinished()
{
    qDebug()<<"Stopped stable diffusion";
    isModelLoaded = false;
    emit isModelLoadedChanged();
}

void TextToImageBackend::openOutputFolder()
{
    if (samplesPath.toString().contains("default")) {
        Utils::openLocalFolderPath(m_options->saveDir());
        return;
    }
    Utils::openLocalFolderPath(samplesPath.toString());
}

void TextToImageBackend::setOutputFolder(QUrl url)
{
    emit setOutputDirectory(url.toLocalFile());
}

void TextToImageBackend::generatingImages()
{
    updateStatusMessage("Generating images...");
    isModelLoaded = true;
    emit isModelLoadedChanged();
}

void TextToImageBackend::imagesGenerated()
{
    updateCompleted();
}

void TextToImageBackend::updateCompleted()
{
    qDebug()<<"Images generated.";
    isProcessing = false;
    updateStatusMessage("Completed.");
    samplesPath = stableDiffusion->getSamplesPath();
    emit isProcessingChanged();
    emit samplesPathChanged();
}

void TextToImageBackend::openLogs()
{
    Utils::openLocalFolderPath(Utils::pathAppend(qApp->applicationDirPath(),LOG_FILE_NAME));
}

void TextToImageBackend::downloadModel()
{

    if (Utils::checkPathExists(diffusionEnv->getStableDiffusionModelPath())){
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("Model file already exists."));
        msgBox.setInformativeText(tr("Do you want to download it again?"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        int ret = msgBox.exec();
        if (ret== QMessageBox::No)
            return;
    }
    if ( !modelDownloader ){
        modelDownloader = new InstallerProcess(this,diffusionEnv);
        connect(modelDownloader, SIGNAL(gotConsoleLog(QString)), this, SLOT(updateDownloaderStatusMessage(QString)));
        connect(modelDownloader, SIGNAL(installCompleted(int,bool)), this, SLOT(installCompleted(int,bool)));
    }

    emit setupInstallerUi(true);
    modelDownloader->downloadStableDiffusionModel();
}

void TextToImageBackend::installPythonEnv()
{
    if ( !pythonEnvInstaller ) {
        pythonEnvInstaller = new InstallerProcess(this,diffusionEnv);
        connect(pythonEnvInstaller, SIGNAL(installCompleted(int,bool)), this, SLOT(installCompleted(int,bool)));
    }
    pythonEnvInstaller->installPipPackages();

}

void TextToImageBackend::stopInstaller()
{
  pythonEnvInstaller->stopProcess();
}

void TextToImageBackend::stopDownloader()
{
    modelDownloader->stopProcess();
}

void TextToImageBackend::cudaMemoryError()
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Critical);
    if (m_options->imageToImage() && !m_options->fitImage())
        msgBox.setText(tr("CUDA memory error: Please reduce initialization image size or enable fit image in settings"));
    else if (m_options->imageToImage() && m_options->fitImage())
         msgBox.setText(tr("CUDA memory error: Please reduce image size in settings"));
    else
        msgBox.setText(tr("CUDA memory error: Failed to generate image,please reduce image size."));

    msgBox.exec();

}

void TextToImageBackend::environmentCurrentStatus(bool isPackagesReady, bool isStableDiffusionModelReady)
{
    handlePackagesStatus(isPackagesReady);
     //SD 1.4 model always available
    handleModelStatus(true);
    initAppControls(isPackagesReady,isStableDiffusionModelReady);

}

void TextToImageBackend::handlePackagesStatus(bool isPackagesReady)
{
    if (!isPackagesReady) {
        qDebug()<<"Environment is not ready,setting it up...";
        installPythonEnv();
        emit installerStatusChanged("Setting up,please wait...",0.0);
    } else {
        qDebug()<<"Environment check : OK";
        m_options->setAppPath(qApp->applicationDirPath());
        emit closeLoadingScreen();
    }
}

void TextToImageBackend::handleModelStatus(bool isStableDiffusionModelReady)
{

    if (isStableDiffusionModelReady){
        qDebug()<<"Stable diffusion original model(v1.4) check : OK ";

    } else {
        qDebug()<<"Stable diffusion original model(v1.4) check: Failed ";
        emit environmentNotReady();
    }
}

void TextToImageBackend::downloadGfpganModel()
{
    setupDownlodUi();
    modelDownloader->downloadGfpganModel();
}

void TextToImageBackend::setImageInput(QUrl url)
{
    emit setInputImagePath(url.toLocalFile());
}

void TextToImageBackend::setMaskImageInput(QUrl url)
{
    emit setInputMaskImagePath(url.toLocalFile());
}

void TextToImageBackend::diffusionCancelled()
{
    updateStatusMessage(tr("Stopped image generation."));
}

void TextToImageBackend::updateDeviceInfo(QString deviceInfo)
{
    qDebug()<<"Device : " << deviceInfo;
    updateStatusMessage(deviceInfo);
}

void TextToImageBackend::initAppControls(bool packageReady, bool stableDiffusionModelReady)
{
    m_envStatus->setIsPythonEnvReady(packageReady);
    m_envStatus->setIsStableDiffusionModelReady(stableDiffusionModelReady);
    m_envStatus->setIsGfpGanModelReady(envValidator->validateGfpGanModel());
    m_envStatus->setIsCodeFormerModelReady(envValidator->validateCodeFormerModel());
    emit initControls(m_options,m_envStatus);
}

void TextToImageBackend::showTermsWindow()
{
    if (!m_options->acceptedTerms()) {
        QString terms = Utils::getTextFileContents(diffusionEnv->getTermsFilePath());
        emit showTermsDialog(terms);
    }
}
const QStringList &TextToImageBackend::getTiConcepts() const
{
    return tiConcepts;
}

void TextToImageBackend::setTiConcepts(const QStringList &newTiConcepts)
{
    tiConcepts = newTiConcepts;
}

bool TextToImageBackend::getIsModelLoaded() const
{
    return isModelLoaded;
}

void TextToImageBackend::setIsModelLoaded(bool newIsModelLoaded)
{
    isModelLoaded = newIsModelLoaded;
}

bool TextToImageBackend::getIsCancelled() const
{
    return isCancelled;
}

void TextToImageBackend::setIsCancelled(bool newIsCancelled)
{
    isCancelled = newIsCancelled;
}

void TextToImageBackend::generateVariations(QUrl imagePath)
{
    QString seedNumber = getSeedFromFileName(imagePath);
    qDebug()<<"Generate variations :"<<seedNumber;
    m_options->setSeed(seedNumber);
    generateImage(true);
    emit showDreamPage();

}

void TextToImageBackend::downloadCodeFormerModel()
{
    setupDownlodUi();
    modelDownloader->downloadCodeFormerModel();
}

DiffusionEnvironmentStatus *TextToImageBackend::envStatus() const
{
    return m_envStatus;
}

void TextToImageBackend::setEnvStatus(DiffusionEnvironmentStatus *newEnvStatus)
{
    m_envStatus = newEnvStatus;
}

bool TextToImageBackend::getIsProcessing() const
{
    return isProcessing;
}

void TextToImageBackend::setIsProcessing(bool newIsProcessing)
{
    isProcessing = newIsProcessing;
}

void TextToImageBackend::updateStatusMessage(const QString &message)
{
    qDebug()<<message;

    if (message.contains(">"))
        return;

    if (message.contains(":/"))
        return;

    diffusionStatusMsg = message;
    emit statusChanged();
}

DiffusionOptions *TextToImageBackend::options() const
{
    return m_options;
}

void TextToImageBackend::setOptions(DiffusionOptions *newOptions)
{
    m_options = newOptions;
}

void TextToImageBackend::classBegin()
{
}

void TextToImageBackend::componentComplete()
{
    qDebug()<<"Component ready";
    loadSettings();

}
void TextToImageBackend::updateInstallerStatusMessage(const QString &message)
{
    installerStatusMsg = message;
    qDebug()<<message;
    emit installerStatusChanged(message,0.0);
}

void TextToImageBackend::updateDownloaderStatusMessage(const QString &message)
{
    installerStatusMsg = message;
    qDebug()<<message;
    emit downloaderStatusChanged(message,modelDownloader->getDownloadProgress());
}

void TextToImageBackend::installCompleted(int exitCode,bool isDownloader)
{
    if (isDownloader) {
        qDebug()<<"Download completed(Exit code) -> "<<exitCode;
        if (Utils::checkPathExists(diffusionEnv->getStableDiffusionModelPath())
                && exitCode == 0) {
            QString msg(tr("Downloaded model successfully,please restart app."));
            qDebug()<<msg;
            emit downloaderStatusChanged(msg,1.0);
        } else {
            QString msg(tr("Model download failed,check logs."));
            qDebug()<<msg;
            emit downloaderStatusChanged(msg,0.0);
        }
    } else {
        if (exitCode == 0) {
            qDebug()<<"Environment is ready.";
            m_options->setAppPath(qApp->applicationDirPath());
        }
        else
            qDebug()<<"Environment is setup failed.";
        emit closeLoadingScreen();
    }
}

QString TextToImageBackend::getSeedFromFileName(QUrl imagePath)
{
    QString seedNum("");
    QFileInfo  imageFile(imagePath.toLocalFile());
    QString fileName = imageFile.fileName();
    QStringList fileNameSplits = fileName.split(".");
    qDebug()<<fileNameSplits;
    if (fileNameSplits.length()>1)
        seedNum = fileNameSplits[1];

    return seedNum;
}

bool TextToImageBackend::isValidInitImage()
{
    return Utils::checkPathExists(m_options->initImagePath());
}

void TextToImageBackend::setupDownlodUi()
{
    if ( !modelDownloader ){
        modelDownloader = new InstallerProcess(this,diffusionEnv);
        connect(modelDownloader, SIGNAL(gotConsoleLog(QString)), this, SLOT(updateDownloaderStatusMessage(QString)));
        connect(modelDownloader, SIGNAL(installCompleted(int,bool)), this, SLOT(installCompleted(int,bool)));
    }

    emit setupInstallerUi(true);
}

void TextToImageBackend::loadTiConceptsNamesFromFolder(const QString& path)
{
    tiConcepts.clear();
    foreach (auto concept, diffusionEnv->getTiConcepFoldertNames(path)) {
        tiConcepts << concept;
    }
    emit tiConceptsChanged();
}

void TextToImageBackend::setTextualInversionFolder(QUrl url)
{
    emit setTiDirectory(url.toLocalFile());
    loadTiConceptsNamesFromFolder(url.toLocalFile());
}

void TextToImageBackend::gotModelsList(QStringList modelsList)
{
    qDebug()<<"Models -" << modelsList;
    stableDiffusionModels = modelsList;
    emit stableDiffusionModelsChanged();
}

void TextToImageBackend::switchModel(QString modelName)
{
   isStableDiffusionModelLoading = true;
   emit isStableDiffusionModelLoadingChanged();
   QString command = "!switch " + modelName;
   stableDiffusion->writeCommand(command);

}

void TextToImageBackend::stableDiffusionModelLoaded(bool isLoaded)
{
   bool isSwitch = isStableDiffusionModelLoading;
   isStableDiffusionModelLoading = false;
   emit isStableDiffusionModelLoadingChanged();

   if (isSwitch & isLoaded) {
       QMessageBox msgBox;
       msgBox.setIcon(QMessageBox::Information);
       msgBox.setText(tr("Model switched successfully"));
       msgBox.exec();
       return ;
   }
   if (!isLoaded) {
       QMessageBox msgBox;
       msgBox.setIcon(QMessageBox::Critical);
       msgBox.setText(tr("Model loading failed,model file not found"));
       msgBox.exec();
   }

}

void TextToImageBackend::copySeedForImage(QUrl imagePath)
{
    QString seedNumber = getSeedFromFileName(imagePath);
    clipboard->setText(seedNumber);
}

bool TextToImageBackend::getIsStableDiffusionModelLoaded() const
{
    return isStableDiffusionModelLoading;
}

void TextToImageBackend::setIsStableDiffusionModelLoaded(bool newIsStableDiffusionModelLoaded)
{
    isStableDiffusionModelLoading = newIsStableDiffusionModelLoaded;
}

const QStringList &TextToImageBackend::getStableDiffusionModels() const
{
    return stableDiffusionModels;
}

void TextToImageBackend::setStableDiffusionModels(const QStringList &newStableDiffusionModels)
{
    stableDiffusionModels = newStableDiffusionModels;

}
