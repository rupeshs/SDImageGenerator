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

#ifndef DIFFUSIONENVVALIDATOR_H
#define DIFFUSIONENVVALIDATOR_H

#include "diffusionenvironment.h"
#include "utils.h"
#include "installer/pythonenvvalidator.h"
#include <QObject>


class DiffusionEnvValidator : public QObject
{
    Q_OBJECT
public:
    explicit DiffusionEnvValidator(QObject *parent = nullptr, DiffusionEnvironment *diffusionEnv = nullptr);

    void ValidatePythonPackages();
    bool validateStableDiffusionModel();
    bool validateGfpGanModel();


    const QString &getDeviceInfo() const;

public slots:
    void packageValidationCompleted(int,bool);
    void gotDeviceInfo(const QString &devInfo);

signals:
    void environmentCurrentStatus(bool isPythonReady, bool isModelReady);

private:
    DiffusionEnvironment *diffusionEnv;
    PythonEnvValidator *pipValidator;
    QString deviceInfo;

    bool validateModelPath();
    bool validateModelFileSize();
    bool validateGfpganModelPath();
    bool validateGfpganModelFileSize();
};

#endif // DIFFUSIONENVVALIDATOR_H
