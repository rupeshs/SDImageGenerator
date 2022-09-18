#include "diffusionprocess.h"
#include <QDebug>

DiffusionProcess::DiffusionProcess(QObject *parent,DiffusionEnvironment *diffusionEnv)
    : QObject{parent}
{

    stableDiffusionProcess=new QProcess(parent);
    stableDiffusionProcess->setProcessChannelMode( QProcess::MergedChannels );
    dreamProcess = new MyProcess(parent);

    connect(dreamProcess,SIGNAL(lineAvailable(QByteArray)),this,SLOT(readProcessOutput(QByteArray)));
    connect(dreamProcess,SIGNAL(finished(int)),this,SLOT(processFinished(int)));

    //connect(stableDiffusionProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readProcessOutput()));
    //connect(stableDiffusionProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)));
    //connect(stableDiffusionProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processError(QProcess::ProcessError)));

    qDebug()<<"DiffusionProcess:: CondaActivatePath"<<diffusionEnv->getCondaActivatePath();
    qDebug()<<"DiffusionProcess:: PythonEnvPath"<<diffusionEnv->getPythonEnvPath();
    qDebug()<<"DiffusionProcess:: StableDiffusionPath"<<diffusionEnv->getStableDiffusionPath();
    dreamProcess->setWorkingDirectory(diffusionEnv->getStableDiffusionPath());

    //exePath = diffusionEnv->getCondaActivatePath();
    //dreamProcess->setProgram(diffusionEnv->getCondaActivatePath());
    addArgument(diffusionEnv->getCondaActivatePath());
    addArgument("&&");
    addArgument("conda");
    addArgument("activate");
    addArgument( diffusionEnv->getPythonEnvPath());
    addArgument("&&");
    addArgument("python");
    addArgument(diffusionEnv->getStableDiffusionScript());
    addArgument("--prompt_as_dir");


}

void DiffusionProcess::readProcessOutput(QByteArray line)
{
    QString consoleLine(line);
    qDebug()<<consoleLine;
    if (!consoleLine.isEmpty())
        emit diffusionConsoleLine(consoleLine);
}

void DiffusionProcess::processFinished(int exit_code, QProcess::ExitStatus exit_status)
{
    qDebug() << "DiffusionProcess::processFinished: exit_code:" << exit_code << "exit_status:" << exit_status;
    emit diffusionFinished();
}
void DiffusionProcess::processError(QProcess::ProcessError error) {

    qDebug() << "DiffusionProcess::processError:" << error;
    if (error == QProcess::FailedToStart) {
        qDebug("DiffusionProcess::processError: process failed to start");
    }
}

void DiffusionProcess::writeCommand(const QString &command)
{
    QString commandToDream = command + QString("\n");
    if (dreamProcess->isRunning())
        dreamProcess->write(commandToDream.toLatin1());
}

void DiffusionProcess::startDreaming()
{
    if (dreamProcess->isRunning()){
        writeCommand(promptCommand);
    }
    else {

        startProcess();
    }
}

const QString &DiffusionProcess::getPromptCommand() const
{
    return promptCommand;
}

void DiffusionProcess::startProcess()
{
    //stableDiffusionProcess->start(exePath,arguments);
    qDebug()<<dreamProcess->arguments();
    dreamProcess->start();
}

void DiffusionProcess::stopProcess()
{

    writeCommand("q");
    /*if (stableDiffusionProcess) {
        stableDiffusionProcess->terminate();
        if (!stableDiffusionProcess->waitForFinished(2000)) {
            stableDiffusionProcess->kill();
            stableDiffusionProcess->waitForFinished();
        }
    }*/
}

void DiffusionProcess::generateImages(DiffusionOptions *diffusionOptions)
{
    //addArgument("--prompt");
    clearPromptArguments();
    addPromptArguments(diffusionOptions->prompt().trimmed());
    addPromptArguments("--sampler");
    addPromptArguments(diffusionOptions->sampler().trimmed());
    addPromptArguments("--cfg_scale");
    addPromptArguments(QString::number(diffusionOptions->scale()));
    addPromptArguments("--width");
    addPromptArguments(QString::number(diffusionOptions->imageWidth()));
    addPromptArguments("--height");
    addPromptArguments(QString::number(diffusionOptions->imageHeight()));
    //addPromptArguments("--n_iter");
    //addPromptArguments(QString::number(1));
    addPromptArguments("--iterations");
    addPromptArguments(QString::number(diffusionOptions->numberOfImages()));
    addPromptArguments("--steps");
    addPromptArguments(QString::number(diffusionOptions->ddimSteps()));

    if (diffusionOptions->seed()>0){
        addPromptArguments("--seed");
        addPromptArguments(QString::number(diffusionOptions->seed()));
    }
    if (!dreamProcess->isRunning()){
        addArgument("--outdir");
        addArgument(diffusionOptions->saveDir());
    }

    promptCommand="";
    for (auto argument : promptArguments) {
        promptCommand.append(argument);
        promptCommand.append(" ");
    }
    qDebug()<<promptCommand;
    startDreaming();

}
