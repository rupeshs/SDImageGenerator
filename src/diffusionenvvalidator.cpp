#include "diffusionenvvalidator.h"

DiffusionEnvValidator::DiffusionEnvValidator(QObject *parent,DiffusionEnvironment *diffEnv)
    : QObject{parent}
{
    diffusionEnv = diffEnv;
}

EnvStatus DiffusionEnvValidator::Validate()
{
 if (!validateCondaPath())
     return EnvStatus::CondaNotFound;

 if (!validatePythonEnvPath())
     return EnvStatus::PythonEnvNotFound;

 if (!Utils::checkPathExists(diffusionEnv->getStableDiffusionPath()))
     return EnvStatus::StableDiffusionNotFound;

 if (!validateModelPath())
     return EnvStatus::StableDiffusionModelNotFound;

 return EnvStatus::Ready;

}

bool DiffusionEnvValidator::validateCondaPath()
{
    return Utils::checkPathExists(diffusionEnv->getCondaActivatePath());
}

bool DiffusionEnvValidator::validatePythonEnvPath()
{
    return Utils::checkPathExists(diffusionEnv->getPythonEnvPath());
}

bool DiffusionEnvValidator::validateModelPath()
{
   return Utils::checkPathExists(diffusionEnv->getStableDiffusionModelPath());
}
