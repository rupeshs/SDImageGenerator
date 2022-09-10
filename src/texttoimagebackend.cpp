#include "texttoimagebackend.h"
#include <QDebug>

TextToImageBackend::TextToImageBackend(QObject *parent)
    : QObject{parent}
{

   diffusionEnv = new DiffusionEnvironment(parent);
   diffusionEnv->getEnvironment();
   verifyEnvironment();

   stableDiffusion = new DiffusionProcess(parent,diffusionEnv);
   connect(stableDiffusion, SIGNAL(diffusionConsoleLine(QString)), this, SLOT(diffusionConsoleLine(QString)));
   samplesPath=  "";


}

void TextToImageBackend::generateImage()
{
    qDebug()<<"Start";
    stableDiffusion->startProcess();
    updateStatusMessage("Starting image generation...");
    //samplesPath="file:/d:\\textimg\\apps\\sdinstaller\\stable-diffusion\\outputs\\txt2img-samples\\Cyberpunk_style_image_of_a_Telsa_car_reflection_in_rain";
    //emit loadImage();
}

void TextToImageBackend::stopProcessing()
{
   stableDiffusion->stopProcess();

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
void TextToImageBackend::verifyEnvironment()
{
    DiffusionEnvValidator envValidator(this,diffusionEnv);
    EnvStatus envStatus = envValidator.Validate();

    if (envStatus == EnvStatus::Ready)
        return;

    if (envStatus == EnvStatus::CondaNotFound)
         errorMsg=tr("Miniconda not found!");

    if (envStatus == EnvStatus::PythonEnvNotFound)
         errorMsg=tr("Python environment not found!");

    if (envStatus == EnvStatus::StableDiffusionNotFound)
         errorMsg=tr("Stable-diffusion directory not found!");

    emit showMessageBox();
}

void TextToImageBackend::updateStatusMessage(const QString &message)
{
    diffusionStatusMsg = message;
    emit statusChanged();
}
