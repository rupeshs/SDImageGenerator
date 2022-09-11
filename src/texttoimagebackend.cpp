#include "texttoimagebackend.h"
#include <QDebug>
#include <QGuiApplication>

TextToImageBackend::TextToImageBackend(QObject *parent)
    : QObject{parent}
{

   diffusionEnv = new DiffusionEnvironment(parent);
   diffusionEnv->getEnvironment();
   verifyEnvironment();

   stableDiffusion = new DiffusionProcess(parent,diffusionEnv);
   connect(stableDiffusion, SIGNAL(diffusionConsoleLine(QString)), this, SLOT(diffusionConsoleLine(QString)));
   m_options = new DiffusionOptions();
   m_options->setPrompt("ddsfsdf");
   isProcessing = false;

}

void TextToImageBackend::generateImage()
{

    qDebug()<<"Start"<<isProcessing;

    if (isProcessing) {
        stopProcessing();
    } else {
        updateStatusMessage("Starting image generation...");

        if (m_options->prompt().isEmpty()) {
            showErrorDlg(tr("Please provide a prompt text."));
            return;
        }
        stableDiffusion->generateImages(m_options);
        qDebug()<<"Prompt : "<<m_options->prompt();
        qDebug()<<"Scale : "<<m_options->scale();
        qDebug()<<"Image width :"<<m_options->imageWidth();
        qDebug()<<"Image height :"<<m_options->imageHeight();
        qDebug()<<"Number of Images to generate :"<<m_options->numberOfImages();
        qDebug()<<"DDIM steps :"<<m_options->ddimSteps();
        qDebug()<<"Sampler :"<<m_options->sampler();
    }
    isProcessing = !isProcessing;

}

void TextToImageBackend::stopProcessing()
{
   stableDiffusion->stopProcess();
   //qApp->exit( TextToImageBackend::EXIT_CODE_REBOOT );

}

void TextToImageBackend::diffusionConsoleLine(QString message)
{

    updateStatusMessage(message);

    if (message.contains("SAMPLEPATH:")) {
       QStringList output_path = message.split("SAMPLEPATH:");
       //QString a = Utils::pathAppend(diffusionEnv->getStableDiffusionPath(),STABLE_DIFFUSION_DEFAULT_OUTPUT_PATH);
       QString b =  Utils::pathAppend(diffusionEnv->getStableDiffusionPath(),output_path[1]);
       samplesPath=  "file:/"+b;
       emit loadImage();
       qDebug()<<output_path[1];
       qDebug()<<samplesPath;
    }
}

void TextToImageBackend::showErrorDlg(const QString &error)
{
    errorMsg = error;
    emit gotErrorMessage();
    emit showMessageBox();
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
