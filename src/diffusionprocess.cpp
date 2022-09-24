#include "diffusionprocess.h"
#include <QDebug>

DiffusionProcess::DiffusionProcess(QObject *parent,DiffusionEnvironment *diffusionEnv)
    : QObject{parent}
{

    dreamProcess = new MyProcess(parent);
    connect(dreamProcess,SIGNAL(lineAvailable(QByteArray)),this,SLOT(readProcessOutput(QByteArray)));
    connect(dreamProcess,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(processFinished(int,QProcess::ExitStatus)));

    qDebug()<<"CondaActivatePath"<<diffusionEnv->getCondaActivatePath();
    qDebug()<<"PythonEnvPath"<<diffusionEnv->getPythonEnvPath();
    qDebug()<<"StableDiffusionPath"<<diffusionEnv->getStableDiffusionPath();
    dreamProcess->setWorkingDirectory(diffusionEnv->getStableDiffusionPath());

    addArgument(diffusionEnv->getCondaActivatePath());
    addArgument("&&");
    addArgument("conda");
    addArgument("activate");
    addArgument( diffusionEnv->getPythonEnvPath());
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

    if (consoleLine.contains("Outputs:")) {
        emit imagesGenerated();
    }
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
    //dreamProcess->sto

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
