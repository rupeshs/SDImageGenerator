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

#include "pythonenvvalidator.h"

PythonEnvValidator::PythonEnvValidator(QObject *parent,DiffusionEnvironment *diffEnv)
    : QObject{parent}
{
    pipProcess = new MyProcess(parent);
    connect(pipProcess,SIGNAL(lineAvailable(QByteArray)),this,SLOT(readProcessOutput(QByteArray)));
    connect(pipProcess,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(processFinished(int,QProcess::ExitStatus)));
    diffusionEnv = diffEnv;
    packages.append("clip");
    packages.append("k-diffusion");
    packages.append("gfpgan");
    packages.append("taming-transformers");
    packages.append("latent-diffusion");
    pipProcess->setWorkingDirectory(diffusionEnv->getStableDiffusionPath());
    hasImportError = false;

}

void PythonEnvValidator::validatePackages()
{

    pipProcess->clearArguments();
    pipProcess->addArgument(diffusionEnv->getCondaActivatePath());
    pipProcess->addArgument("&&");
    pipProcess->addArgument("conda");
    pipProcess->addArgument("activate");
    pipProcess->addArgument("sdimgenv");
    pipProcess->addArgument("&&");
    pipProcess->addArgument("python");
    pipProcess->addArgument(diffusionEnv->getLibsTestScriptPath());

    pipProcess->start();

}

void PythonEnvValidator::readProcessOutput(QByteArray line)
{
    QString consoleLine(line);
    if (consoleLine.contains("ImportError"))
        hasImportError = true;

}

void PythonEnvValidator::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "PythonEnvValidator: exitCode:" << exitCode << "exitStatus:" << exitStatus;
    bool packagesReady = !hasImportError;
    emit packageValidationCompleted(exitCode, packagesReady);

}

void PythonEnvValidator::processError(QProcess::ProcessError error)
{
    qDebug() << "PythonEnvValidator ProcessError:" << error;
    if (error == QProcess::FailedToStart) {
        qDebug()<<"PythonEnvValidator failed to start";
    }
}

bool PythonEnvValidator::validate()
{
    bool result = true;
    for (const QString package : packages) {
        result &= pipListOutput.contains(package);
    }
    return result;
}
