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

#include "diffusionprocess.h"
#include <QDebug>

DiffusionProcess::DiffusionProcess(QObject *parent,DiffusionEnvironment *diffusionEnv)
    : QObject{parent}
{

    dreamProcess = new MyProcess(parent);
    connect(dreamProcess,SIGNAL(lineAvailable(QByteArray)),this,SLOT(readProcessOutput(QByteArray)));
    connect(dreamProcess,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(processFinished(int,QProcess::ExitStatus)));

    qDebug()<<"CondaActivatePath"<<diffusionEnv->getCondaActivatePath();
    qDebug()<<"StableDiffusionPath"<<diffusionEnv->getStableDiffusionPath();
    dreamProcess->setWorkingDirectory(diffusionEnv->getStableDiffusionPath());
    qDebug()<<"Directory"<<dreamProcess->workingDirectory();

    addArgument(diffusionEnv->getCondaActivatePath());
    addArgument("&&");
    addArgument("conda");
    addArgument("activate");
    addArgument("sdimgenv");
    addArgument("&&");
    addArgument("python");
    addArgument(diffusionEnv->getStableDiffusionScript());
    addArgument("--prompt_as_dir");

    setStatus(StableDiffusionStatus::NotStarted);
}

void DiffusionProcess::readProcessOutput(QByteArray line)
{
    QString consoleLine(line);

    if (!line.isEmpty())
        emit gotConsoleLog(line);

    if (consoleLine.contains("conda.bat"))
       emit gotConsoleLog(QString("Initializing,please wait..."));

    if(rxOutputFolder.indexIn(consoleLine)>-1){
       QString outFolderPath = rxOutputFolder.cap(1);
       samplesPath = Utils::localPathToUrl(outFolderPath);
       emit generatingImages();
    }

    if (consoleLine.contains("Awaiting your command")) {
         qDebug()<<getPromptCommand();
         writeCommand(getPromptCommand());
         emit generatingImages();
    }

    if (consoleLine.contains("Outputs:"))
        emit imagesGenerated();


    if (consoleLine.contains("RuntimeError: CUDA out of memory"))
        emit cudaMemoryError();

}

void DiffusionProcess::processFinished(int exit_code, QProcess::ExitStatus exit_status)
{
    qDebug() << "exit_code:" << exit_code << "exit_status:" << exit_status;
    emit diffusionFinished();
}
void DiffusionProcess::processError(QProcess::ProcessError error) {

    qDebug() << "ProcessError:" << error;
    if (error == QProcess::FailedToStart) {
        qDebug()<<"Process failed to start";
    }
}

void DiffusionProcess::writeCommand(const QString &command)
{
    QString commandToDream = command + QString("\n");
    if (dreamProcess->isRunning())
        dreamProcess->write(commandToDream.toLatin1());
}

const QUrl &DiffusionProcess::getSamplesPath() const
{
    return samplesPath;
}

StableDiffusionStatus DiffusionProcess::getStatus() const
{
    return status;
}

void DiffusionProcess::setStatus(StableDiffusionStatus newStatus)
{
    status = newStatus;
}

void DiffusionProcess::addDreamScriptArgs(DiffusionOptions *diffusionOptions)
{
    if (!dreamProcess->isRunning()){
        // Main options
        addArgument("--outdir");
        addArgument(diffusionOptions->saveDir());

        if (diffusionOptions->fullPrecision())
            addArgument("--full_precision");
    }
}

void DiffusionProcess::addSaveOriginalImageArg(bool saveImage)
{
    if (saveImage)
        addPromptArguments("--save_original");
}

void DiffusionProcess::addDimensionArgs(qreal width,qreal height)
{
    addPromptArguments("--width");
    addPromptArguments(QString::number(width));
    addPromptArguments("--height");
    addPromptArguments(QString::number(height));
}

void DiffusionProcess::startDreaming()
{
    if (dreamProcess->isRunning())
        writeCommand(promptCommand);
    else
        startProcess();
}

const QString &DiffusionProcess::getPromptCommand() const
{
    return promptCommand;
}

void DiffusionProcess::startProcess()
{
    qDebug()<<dreamProcess->arguments();
    dreamProcess->start();
    setStatus(StableDiffusionStatus::Starting);
}

void DiffusionProcess::stopProcess()
{
    writeCommand("q");

    if (dreamProcess) {
        dreamProcess->terminate();
        if (!dreamProcess->waitForFinished(2000)) {
            dreamProcess->kill();
            dreamProcess->waitForFinished();
        }
    }
}

void DiffusionProcess::generateImages(DiffusionOptions *diffusionOptions)
{
    //addArgument("--prompt");
    clearPromptArguments();
    addPromptArguments(diffusionOptions->prompt().trimmed());
    if (!diffusionOptions->imageToImage()) {
    addPromptArguments("--sampler");
    addPromptArguments(diffusionOptions->sampler().trimmed());
    addDimensionArgs(diffusionOptions->imageWidth(),diffusionOptions->imageHeight());
    }
    else {
        if (diffusionOptions->fitImage())
            addDimensionArgs(diffusionOptions->imageWidth(),diffusionOptions->imageHeight());
    addPromptArguments("--init_mask");
    addPromptArguments("C:/Users/Rupesh/Downloads/mask.png");
    }
    addPromptArguments("--cfg_scale");
    addPromptArguments(QString::number(diffusionOptions->scale()));
    addPromptArguments("--iterations");
    addPromptArguments(QString::number(diffusionOptions->numberOfImages()));
    addPromptArguments("--steps");
    addPromptArguments(QString::number(diffusionOptions->ddimSteps()));

    if (diffusionOptions->grid())
        addPromptArguments("--grid");

    if (diffusionOptions->seamless())
        addPromptArguments("--seamless");

    if (!diffusionOptions->seed().isEmpty()){
        addPromptArguments("--seed");
        addPromptArguments(diffusionOptions->seed());
    }

    if (diffusionOptions->upscaler() || diffusionOptions->faceRestoration())
        addSaveOriginalImageArg(diffusionOptions->saveOriginalImage());

    if (diffusionOptions->upscaler()) {
        addPromptArguments("--upscale");
        QString upscaleFactor = diffusionOptions->upscaleFactor();
        upscaleFactor.chop(1);
        addPromptArguments(upscaleFactor);
        addPromptArguments(QString::number(diffusionOptions->upscaleStrength()));

    }
    if (diffusionOptions->faceRestoration()) {
        addPromptArguments("--gfpgan_strength");
        addPromptArguments(QString::number(diffusionOptions->faceRestorationStrength()));
    }

    if (diffusionOptions->imageToImage()) {
        addPromptArguments("--init_img");
        addPromptArguments(diffusionOptions->initImagePath());
        if (diffusionOptions->fitImage())
            addPromptArguments("--fit");
        addPromptArguments("--strength");
        addPromptArguments(QString::number(diffusionOptions->imageToImageStrength()));
    }

    addDreamScriptArgs(diffusionOptions);

    promptCommand="";
    foreach (QString argument , promptArguments) {
        promptCommand.append(argument);
        promptCommand.append(" ");
    }
    qDebug()<<promptCommand;

    startDreaming();
}
