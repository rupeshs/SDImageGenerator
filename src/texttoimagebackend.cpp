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

    deafultAssetsPath = Utils::pathAppend(qApp->applicationDirPath(),"default");
    samplesPath = Utils::localPathToUrl(deafultAssetsPath);

    envValidator = new DiffusionEnvValidator(this,diffusionEnv);
    connect(envValidator, SIGNAL(environmentCurrentStatus(bool,bool)), this, SLOT(environmentCurrentStatus(bool,bool)));
    envValidator->Validate();

    m_envStatus = new DiffusionEnvironmentStatus();

    modelDownloader = nullptr;
    pythonEnvInstaller = nullptr;
}

void TextToImageBackend::generateImage()
{
    if (!Utils::checkPathExists(m_options->saveDir())) {
        showErrorDlg(tr("Please choose a output directory from settings tab."));
        return;
    }

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
    qInfo()<<"Seed :"<< m_options->seed();
    qInfo()<<"Save dir :"<< m_options->saveDir();
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
    qDebug()<<"Save StableDiffusion settings : "<<QString::number(m_options->metaObject()->propertyCount());
    qDebug()<<m_options->seed();
    settings->beginGroup("Main");
    settings->setValue("prompt", m_options->prompt());
    settings->setValue("scale", m_options->scale());
    settings->setValue("imageWidth", m_options->imageWidth());
    settings->setValue("imageHeight", m_options->imageHeight());
    settings->setValue("numberOfImages", m_options->numberOfImages());
    settings->setValue("ddimSteps", m_options->ddimSteps());
    settings->setValue("sampler", m_options->sampler());
    settings->setValue("seed", m_options->seed());
    settings->setValue("saveDir", m_options->saveDir());
    settings->setValue("grid", m_options->grid());
    settings->endGroup();
}

void TextToImageBackend::loadSettings()
{
    qInfo()<<"Loading app settings";
    m_options->setPrompt(settings->value("Main/prompt","").toString());
    m_options->setScale(settings->value("Main/scale",DEFAULT_SCALE).toDouble());
    m_options->setImageWidth(settings->value("Main/imageWidth",DEFAULT_IMAGE_WIDTH).toDouble());
    m_options->setImageHeight(settings->value("Main/imageHeight",DEFAULT_IMAGE_HEIGHT).toDouble());
    m_options->setNumberOfImages(settings->value("Main/numberOfImages",DEFAULT_NUMBER_OF_IMAGES).toDouble());
    m_options->setDdimSteps(settings->value("Main/ddimSteps",DEFAULT_DDIM_STEPS).toDouble());
    m_options->setSampler(settings->value("Main/sampler",DEFAULT_SAMPLER).toString());
    m_options->setSaveDir(settings->value("Main/saveDir",diffusionEnv->getDefaultOutDir()).toString());
    QString seed = settings->value("Main/seed",DEFAULT_SEED).toString();
    m_options->setSeed(seed);

    Utils::ensurePath(m_options->saveDir());
    m_options->setGrid(settings->value("Main/grid",DEFAULT_GRID).toBool());
    emit setupInstallerUi(false);

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
    m_options->setSaveDir(diffusionEnv->getDefaultOutDir());
    m_options->setGrid(DEFAULT_GRID);

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

        if (envValidator->validateModelFileSize())
            msgBox.setText(tr("Model file already exists."));
        else
            msgBox.setText(tr("Model file size does not match!"));

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
    qDebug()<<diffusionEnv->getCurlPath();
    qDebug()<<diffusionEnv->getStableDiffusionModelPath();
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
    msgBox.setText(tr("CUDA memory error: Failed to generate image,please reduce image size."));
    msgBox.exec();

}

void TextToImageBackend::environmentCurrentStatus(bool isPackagesReady, bool isStableDiffusionModelReady)
{
    handlePackagesStatus(isPackagesReady);
    handleModelStatus(isStableDiffusionModelReady);

    m_envStatus->setIsPythonEnvReady(isPackagesReady);
    m_envStatus->setIsStableDiffusionModelReady(isStableDiffusionModelReady);
    emit initControls(m_options,m_envStatus);
}

void TextToImageBackend::handlePackagesStatus(bool isPackagesReady)
{
    if (!isPackagesReady) {
        qDebug()<<"Environment is not ready,setting it up...";
        installPythonEnv();
    }
    else{
        qDebug()<<"Environment check : OK";
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
    //verifyEnvironment();
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
        if ( exitCode == 0 )
            qDebug()<<"Environment is ready.";
        else
            qDebug()<<"Environment is setup failed.";
        emit closeLoadingScreen();
    }
}
