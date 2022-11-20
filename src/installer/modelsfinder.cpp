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

#include "modelsfinder.h"

ModelsFinder::ModelsFinder(QObject *parent,DiffusionEnvironment *diffEnv)
    : QObject{parent}
{
    modelsFinderProcess = new MyProcess(parent);
    connect(modelsFinderProcess,SIGNAL(lineAvailable(QByteArray)),this,SLOT(readProcessOutput(QByteArray)));
    connect(modelsFinderProcess,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(processFinished(int,QProcess::ExitStatus)));
    diffusionEnv = diffEnv;
    modelsFinderProcess->setWorkingDirectory(diffusionEnv->getStableDiffusionPath());
}

void ModelsFinder::find()
{
    modelsFinderProcess->clearArguments();
    modelsFinderProcess->addArgument(diffusionEnv->getCondaActivatePath());
    modelsFinderProcess->addArgument("&&");
    modelsFinderProcess->addArgument("conda");
    modelsFinderProcess->addArgument("activate");
    modelsFinderProcess->addArgument("sdimgenv");
    modelsFinderProcess->addArgument("&&");
    modelsFinderProcess->addArgument("python");
    modelsFinderProcess->addArgument(diffusionEnv->getModelsFinderScriptPath());

    modelsFinderProcess->start();
}

void ModelsFinder::readProcessOutput(QByteArray line)
{
    QString consoleLine(line);
    QStringList modelNameSplit =  consoleLine.split(",");
    if (line.contains("sdmodel") && modelNameSplit.length()==2) {
        stableDiffusionModels<<modelNameSplit[1];
    }
}

void ModelsFinder::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "ModelsFinder: exitCode:" << exitCode << "exitStatus:" << exitStatus;
    emit gotModelsList(stableDiffusionModels);
}

void ModelsFinder::processError(QProcess::ProcessError error)
{
    qDebug() << "ModelsFinder ProcessError:" << error;
    if (error == QProcess::FailedToStart) {
        qDebug()<<"ModelsFinder failed to start";
    }
}
