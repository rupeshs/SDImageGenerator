#include "texttoimagebackend.h"
#include <QDebug>
#include <QGuiApplication>
#include <QDesktopServices>

TextToImageBackend::TextToImageBackend(QObject *parent)
    : QObject{parent}
{
    isProcessing = false;
    settings = new QSettings(qApp->applicationDirPath()+"/sdimagegenerator.ini",QSettings::IniFormat,this);

    m_options = new DiffusionOptions();
    diffusionEnv = new DiffusionEnvironment(parent);
    diffusionEnv->getEnvironment();
    stableDiffusion = new DiffusionProcess(parent,diffusionEnv);
    connect(stableDiffusion, SIGNAL(diffusionConsoleLine(QString)), this, SLOT(diffusionConsoleLine(QString)));
    connect(stableDiffusion, SIGNAL(diffusionFinished()), this, SLOT(stableDiffusionFinished()));

}

void TextToImageBackend::generateImage()
{
    qDebug()<<"Start"<<isProcessing;

    updateStatusMessage("Starting image generation...");
    if (m_options->prompt().isEmpty()) {
        showErrorDlg(tr("Please provide a prompt text."));
        return;
    }
    stableDiffusion->generateImages(m_options);
    qDebug()<<"Prompt : "<<m_options->prompt().trimmed();
    qDebug()<<"Scale : "<<m_options->scale();
    qDebug()<<"Image width :"<<m_options->imageWidth();
    qDebug()<<"Image height :"<<m_options->imageHeight();
    qDebug()<<"Number of Images to generate :"<<m_options->numberOfImages();
    qDebug()<<"DDIM steps :"<<m_options->ddimSteps();
    qDebug()<<"Sampler :"<<m_options->sampler();
    isProcessing = true;
    emit isProcessingChanged();

}

void TextToImageBackend::stopProcessing()
{
    stableDiffusion->stopProcess();

}

void TextToImageBackend::diffusionConsoleLine(QString consoleLine)
{
   updateStatusMessage(consoleLine);

    if (consoleLine.contains("SAMPLEPATH:")) {
        QStringList outputFolderInfo = consoleLine.split("SAMPLEPATH:");
        if (outputFolderInfo.count()==2)
            samplesPath =  QUrl::fromLocalFile(outputFolderInfo[1]);
    }
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
    m_options->setPrompt(settings->value("StableDiffusion/prompt","").toString());
    m_options->setScale(settings->value("StableDiffusion/scale",DEFAULT_SCALE).toDouble());
    m_options->setImageWidth(settings->value("StableDiffusion/imageWidth",DEFAULT_IMAGE_WIDTH).toDouble());
    m_options->setImageHeight(settings->value("StableDiffusion/imageHeight",DEFAULT_IMAGE_HEIGHT).toDouble());
    m_options->setNumberOfImages(settings->value("StableDiffusion/numberOfImages",DEFAULT_NUMBER_OF_IMAGES).toDouble());
    m_options->setDdimSteps(settings->value("StableDiffusion/ddimSteps",DEFAULT_DDIM_STEPS).toDouble());
    m_options->setSampler(settings->value("StableDiffusion/sampler",DEFAULT_SAMPLER).toString());
    QString defaultOutDir = Utils::pathAppend(qApp->applicationDirPath(),STABLE_DIFFUSION_RESULTS_FOLDER_NAME);
    m_options->setSaveDir(settings->value("StableDiffusion/saveDir",defaultOutDir).toString());

    double seed = settings->value("StableDiffusion/seed",DEFAULT_NUMBER_OF_IMAGES).toDouble();
    if(seed)
       m_options->setSeed(seed);

    emit initControls(m_options);
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

    emit initControls(m_options);
}

void TextToImageBackend::stableDiffusionFinished()
{
    isProcessing = false;
    updateStatusMessage("Completed.");
    emit isProcessingChanged();
    emit samplesPathChanged();
}

void TextToImageBackend::openOutputFolder()
{
   // if(!Utils::checkPathExists(samplesPath.toLocalFile()))
    QDesktopServices::openUrl(samplesPath);
}

void TextToImageBackend::setOutputFolder(QUrl url)
{
    emit setOutputDirectory(url.toLocalFile());
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

void TextToImageBackend::verifyEnvironment()
{
    DiffusionEnvValidator envValidator(this,diffusionEnv);
    EnvStatus envStatus = envValidator.Validate();

    if (envStatus == EnvStatus::Ready)
        return;

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
}

void TextToImageBackend::updateStatusMessage(const QString &message)
{
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
    qDebug()<<"TextToImageBackend: Component ready";
    loadSettings();
    initBackend();
}
