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

#include "diffusionenvvalidator.h"


DiffusionEnvValidator::DiffusionEnvValidator(QObject *parent,DiffusionEnvironment *diffEnv)
    : QObject{parent}
{
    diffusionEnv = diffEnv;
    pipValidator = new PythonEnvValidator(parent,diffEnv);
    connect(pipValidator,SIGNAL(packageValidationCompleted(int,bool)),this,SLOT(packageValidationCompleted(int,bool)));
    connect(pipValidator,SIGNAL(gotDeviceInfo(QString)),this,SLOT(gotDeviceInfo(QString)));
}

void DiffusionEnvValidator::ValidatePythonPackages()
{
    pipValidator->validatePackages();
}

bool DiffusionEnvValidator::validateModelPath()
{
    return Utils::checkPathExists(diffusionEnv->getStableDiffusionModelPath());
}

bool DiffusionEnvValidator::validateModelFileSize()
{
    QFile modelFile(diffusionEnv->getStableDiffusionModelPath());
    return STABLE_DIFFUSION_MODEL_1_4_FILE_SIZE == modelFile.size();
}

bool DiffusionEnvValidator::validateGfpganModelPath()
{
    return Utils::checkPathExists(diffusionEnv->getGfpGanModelPath());
}

bool DiffusionEnvValidator::validateGfpganModelFileSize()
{
    QFile modelFile(diffusionEnv->getGfpGanModelPath());
    return GFP_GAN_MODEL_1_3_FILE_SIZE == modelFile.size();
}

bool DiffusionEnvValidator::validateStableDiffusionModel()
{
    return validateModelPath() && validateModelFileSize();
}

bool DiffusionEnvValidator::validateGfpGanModel()
{
    return validateGfpganModelPath() && validateGfpganModelFileSize();
}

void DiffusionEnvValidator::gotDeviceInfo(const QString &devInfo)
{
    deviceInfo = devInfo;
}

void DiffusionEnvValidator::packageValidationCompleted(int , bool isPackagesReady)
{
    emit environmentCurrentStatus(isPackagesReady, validateStableDiffusionModel());
}

const QString &DiffusionEnvValidator::getDeviceInfo() const
{
    return deviceInfo;
}
