#include "texttoimagebackend.h"
#include <QDebug>
#include <QGuiApplication>
#include <QDesktopServices>
#include <QMessageBox>
#include <QTimer>

TextToImageBackend::TextToImageBackend(QObject *parent)
    : QObject{parent}
{
    isProcessing = false;
    settings = new QSettings(qApp->applicationDirPath()+"/sdimagegenerator.ini",QSettings::IniFormat,this);

    m_options = new DiffusionOptions();
    diffusionEnv = new DiffusionEnvironment(parent);
    diffusionEnv->getEnvironment();
    stableDiffusion = new DiffusionProcess(parent,diffusionEnv);
    connect(stableDiffusion, SIGNAL(generatingImages()), this, SLOT(generatingImages()));
    connect(stableDiffusion, SIGNAL(imagesGenerated()), this, SLOT(imagesGenerated()));
    connect(stableDiffusion, SIGNAL(diffusionFinished()), this, SLOT(stableDiffusionFinished()));
    connect(stableDiffusion, SIGNAL(gotConsoleLog(QString)), this, SLOT(updateStatusMessage(QString)));
    connect(stableDiffusion, SIGNAL(cudaMemoryError()), this, SLOT(cudaMemoryError()));

//    QMessageBox msgBox;
//    msgBox.critical(nullptr, "csTitle", "csMsg asddddddd");
//    msgBox.show();
      //QTimer::singleShot(5000, this, &TextToImageBackend::generateImage());

    deafultAssetsPath = Utils::pathAppend(qApp->applicationDirPath(),"default");
    samplesPath = Utils::localPathToUrl(deafultAssetsPath);

    envValidator = new DiffusionEnvValidator(this,diffusionEnv);
    EnvStatus envStatus = envValidator->Validate();

    m_envStatus = new DiffusionEnvironmentStatus();

    //setEnvStatus(m_envStatus);

    if (envStatus != EnvStatus::Ready) {
        qDebug()<<"Environment is not ready,please install it from the install tab";
        QTimer::singleShot(0, this, [this](){emit environmentNotReady();});
    }
    modelDownloader = nullptr;
    pythonEnvInstaller = nullptr;

}

void TextToImageBackend::generateImage()
{
    if (!verifyEnvironment())
        return;

    if (m_options->prompt().isEmpty()) {
        showErrorDlg(tr("Please provide a prompt text."));
        return;
    }

    if (stableDiffusion->getStatus() == StableDiffusionStatus::NotStarted)
        updateStatusMessage(tr("Initializing,please wait..."));
    else
        updateStatusMessage(tr("Starting image generation..."));

    stableDiffusion->generateImages(m_options);
    qInfo()<<"Prompt : "<< m_options->prompt().trimmed();
    qInfo()<<"Scale : "<< m_options->scale();
    qInfo()<<"Image width :"<< m_options->imageWidth();
    qInfo()<<"Image height :"<< m_options->imageHeight();
    qInfo()<<"Number of Images to generate :"<< m_options->numberOfImages();
    qInfo()<<"DDIM steps :"<< m_options->ddimSteps();
    qInfo()<<"Sampler :"<< m_options->sampler();
    isProcessing = true;
    samplesPath = Utils::localPathToUrl(deafultAssetsPath);
    emit samplesPathChanged();
    emit isProcessingChanged();
}

void TextToImageBackend::stopProcessing()
{
    stableDiffusion->stopProcess();

}

void TextToImageBackend::showErrorDlg(const QString &error)
{
    errorMsg = error;
    emit gotErrorMessage();
    emit showMessageBox();
}

void TextToImageBackend::saveSettings()
{
    settings->beginGroup("StableDiffusion");
    qDebug()<<"Save StableDiffusion settings : "<<QString::number(m_options->metaObject()->propertyCount());
    for (int i =m_options->metaObject()->propertyOffset(); i <m_options->metaObject()->propertyCount() ; ++i) {
        QString settingName = m_options->metaObject()->property(i).name();
        QVariant settingValue =  m_options->metaObject()->property(i).read(m_options);
        if ( settingName=="prompt")
            settings->setValue(settingName,settingValue.toString().trimmed());
        else
            settings->setValue(settingName,settingValue);
    }
}

void TextToImageBackend::loadSettings()
{
    qInfo()<<"Loading app settings";
    m_options->setPrompt(settings->value("StableDiffusion/prompt","").toString());
    m_options->setScale(settings->value("StableDiffusion/scale",DEFAULT_SCALE).toDouble());
    m_options->setImageWidth(settings->value("StableDiffusion/imageWidth",DEFAULT_IMAGE_WIDTH).toDouble());
    m_options->setImageHeight(settings->value("StableDiffusion/imageHeight",DEFAULT_IMAGE_HEIGHT).toDouble());
    m_options->setNumberOfImages(settings->value("StableDiffusion/numberOfImages",DEFAULT_NUMBER_OF_IMAGES).toDouble());
    m_options->setDdimSteps(settings->value("StableDiffusion/ddimSteps",DEFAULT_DDIM_STEPS).toDouble());
    m_options->setSampler(settings->value("StableDiffusion/sampler",DEFAULT_SAMPLER).toString());
    QString defaultOutDir = Utils::pathAppend(qApp->applicationDirPath(),STABLE_DIFFUSION_RESULTS_FOLDER_NAME);
    m_options->setSaveDir(settings->value("StableDiffusion/saveDir",defaultOutDir).toString());

    double seed = settings->value("StableDiffusion/seed",DEFAULT_SEED).toDouble();
    if(seed)
        m_options->setSeed(seed);

    m_envStatus->setIsCondaReady(envValidator->validateCondaPath());
    m_envStatus->setIsPythonEnvReady(envValidator->validatePythonEnvPath());
    m_envStatus->setIsStableDiffusionModelReady(envValidator->validateModelPath());

    emit initControls(m_options,m_envStatus);
    Utils::ensurePath(m_options->saveDir());
}

void TextToImageBackend::resetSettings()
{
    m_options->setScale(DEFAULT_SCALE);
    m_options->setImageWidth(DEFAULT_IMAGE_WIDTH);
    m_options->setImageHeight(DEFAULT_IMAGE_HEIGHT);
    m_options->setNumberOfImages(DEFAULT_NUMBER_OF_IMAGES);
    m_options->setDdimSteps(DEFAULT_DDIM_STEPS);
    m_options->setSampler(DEFAULT_SAMPLER);
    m_options->setSeed(DEFAULT_SEED);

    emit initControls(m_options,m_envStatus);
}

void TextToImageBackend::stableDiffusionFinished()
{
    qDebug()<<"Good bye";
}

void TextToImageBackend::openOutputFolder()
{
    QDesktopServices::openUrl(samplesPath);
}

void TextToImageBackend::setOutputFolder(QUrl url)
{
    emit setOutputDirectory(url.toLocalFile());
}

void TextToImageBackend::installEnvironment()
{
#ifdef Q_OS_WIN
   QString exeFileName(qApp->applicationDirPath()+"/install-env.exe");

  ::ShellExecuteA(0, "open", exeFileName.toUtf8().constData(), 0, 0, SW_HIDE);
  /* if (SE_ERR_ACCESSDENIED == result)
   {
       // Requesting elevation(Windows Vista/Window7/window8)
       result = (HINSTANCE)::ShellExecuteA(0, "runas", exeFileName.toUtf8().constData(), 0, 0, SW_HIDE);
   }*/
#endif
}

void TextToImageBackend::generatingImages()
{
    updateStatusMessage("Generating images...");
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
    QDesktopServices::openUrl(Utils::pathAppend(qApp->applicationDirPath(),LOG_FILE_NAME));
}

void TextToImageBackend::downloadModel()
{

    if (Utils::checkPathExists(diffusionEnv->getStableDiffusionModelPath())){
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("Model already exists."));
        msgBox.setInformativeText(tr("Do you want to download it again?"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        int ret = msgBox.exec();
        if (ret== QMessageBox::No)
            return;
    }
    if ( !modelDownloader ){
        modelDownloader = new InstallerProcess(this,diffusionEnv);
        //https://www.googleapis.com/storage/v1/b/aai-blog-files/o/sd-v1-4.ckpt?alt=media
        connect(modelDownloader, SIGNAL(gotConsoleLog(QString)), this, SLOT(updateDownloaderStatusMessage(QString)));
        connect(modelDownloader, SIGNAL(installCompleted(int,bool)), this, SLOT(installCompleted(int,bool)));
    }
    qDebug()<<diffusionEnv->getCurlPath();
    qDebug()<<diffusionEnv->getStableDiffusionModelPath();
    emit setupInstallerUi(true);
    modelDownloader->downloadStableDiffusionModel();
}

void TextToImageBackend::installPythonEnv()
{
    if ( !pythonEnvInstaller ) {
        pythonEnvInstaller = new InstallerProcess(this,diffusionEnv);
        connect(pythonEnvInstaller, SIGNAL(gotConsoleLog(QString)), this, SLOT(updateInstallerStatusMessage(QString)));
        connect(pythonEnvInstaller, SIGNAL(installCompleted(int,bool)), this, SLOT(installCompleted(int,bool)));
    }
    emit setupInstallerUi(false);
    pythonEnvInstaller->installCondaEnv();

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
    msgBox.setText(tr("CUDA memory error: Failed to generate image,please reduce image size."));
    msgBox.exec();

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

void TextToImageBackend::initBackend()
{
    verifyEnvironment();
}

bool TextToImageBackend::verifyEnvironment()
{
    DiffusionEnvValidator envValidator(this,diffusionEnv);
    EnvStatus envStatus = envValidator.Validate();

    if (envStatus == EnvStatus::Ready){
        qDebug()<<"Environment ready!";
        return true;
    }

    if (envStatus == EnvStatus::CondaNotFound){
        qDebug()<<"Miniconda not found!";
        errorMsg=tr("Miniconda not found!");

    }
    if (envStatus == EnvStatus::PythonEnvNotFound) {
        qDebug()<<"Python environment not found!";
        errorMsg=tr("Python environment not found!");
    }

    if (envStatus == EnvStatus::StableDiffusionNotFound) {
        qDebug()<<"Stable-diffusion directory not found!";
        errorMsg=tr("Stable-diffusion directory not found!");
    }
    emit gotErrorMessage();
    emit showMessageBox();

    return false;
}

void TextToImageBackend::updateStatusMessage(const QString &message)
{
    diffusionStatusMsg = message;
    qDebug()<<message;
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
        if ( exitCode == 0 ) {
            QString msg(tr("Installation completed successfully,please restart app."));
            qDebug()<<msg;
            emit installerStatusChanged(msg,0.0);

        }
    }
}
