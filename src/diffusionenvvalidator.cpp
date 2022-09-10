#include "diffusionenvvalidator.h"

DiffusionEnvValidator::DiffusionEnvValidator(QObject *parent,DiffusionEnvironment *diffEnv)
    : QObject{parent}
{
    diffusionEnv = diffEnv;
}

EnvStatus DiffusionEnvValidator::Validate()
{
 if (!Utils::checkPathExists(diffusionEnv->getCondaActivatePath()))
     return EnvStatus::CondaNotFound;

 if (!Utils::checkPathExists(diffusionEnv->getPythonEnvPath()))
     return EnvStatus::PythonEnvNotFound;

 if (!Utils::checkPathExists(diffusionEnv->getStableDiffusionPath()))
     return EnvStatus::StableDiffusionNotFound;

 return EnvStatus::Ready;

}
