#include "diffusionprocess.h"
#include <QDebug>

DiffusionProcess::DiffusionProcess(QObject *parent,DiffusionEnvironment *diffusionEnv)
    : QObject{parent}
{

    stableDiffusionProcess=new QProcess(parent);
    stableDiffusionProcess->setProcessChannelMode( QProcess::MergedChannels );

    connect(stableDiffusionProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readProcessOutput()));
    connect(stableDiffusionProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)));
    connect(stableDiffusionProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processError(QProcess::ProcessError)));

    qDebug()<<"DiffusionProcess:: CondaActivatePath"<<diffusionEnv->getCondaActivatePath();
    qDebug()<<"DiffusionProcess:: PythonEnvPath"<<diffusionEnv->getPythonEnvPath();
    qDebug()<<"DiffusionProcess:: StableDiffusionPath"<<diffusionEnv->getStableDiffusionPath();
    stableDiffusionProcess->setWorkingDirectory(diffusionEnv->getStableDiffusionPath());

    exePath = diffusionEnv->getCondaActivatePath();
    addArgument("&&");
    addArgument("conda");
    addArgument("activate");
    addArgument( diffusionEnv->getPythonEnvPath());
    addArgument("&&");
    addArgument("python");
    addArgument(diffusionEnv->getStableDiffusionScript());

}

void DiffusionProcess::readProcessOutput()
{
    QByteArray line;
    while (stableDiffusionProcess->canReadLine()) {
        line = stableDiffusionProcess->readLine().trimmed();
        qDebug() << "DiffusionProcess::readProcessOutput: line:" << line;
        if (line!="")
            emit diffusionConsoleLine(line);
    }
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

void DiffusionProcess::startProcess()
{
    stableDiffusionProcess->start(exePath,arguments);
}

void DiffusionProcess::stopProcess()
{
    if (stableDiffusionProcess) {
        stableDiffusionProcess->terminate();
        if (!stableDiffusionProcess->waitForFinished(2000)) {
            stableDiffusionProcess->kill();
            stableDiffusionProcess->waitForFinished();
        }
    }
}

void DiffusionProcess::generateImages(DiffusionOptions *diffusionOptions)
{
    addArgument("--prompt");
    addArgument(diffusionOptions->prompt().trimmed());
    addArgument("--sampler");
    addArgument(diffusionOptions->sampler().trimmed());
    addArgument("--scale");
    addArgument(QString::number(diffusionOptions->scale()));
    addArgument("--W");
    addArgument(QString::number(diffusionOptions->imageWidth()));
    addArgument("--H");
    addArgument(QString::number(diffusionOptions->imageHeight()));
    addArgument("--n_iter");
    addArgument(QString::number(1));
    addArgument("--n_samples");
    addArgument(QString::number(diffusionOptions->numberOfImages()));
    addArgument("--ddim_steps");
    addArgument(QString::number(diffusionOptions->ddimSteps()));
    addArgument("--outdir");
    addArgument(diffusionOptions->saveDir());

    if (diffusionOptions->seed()>0){
        addArgument("--seed");
        addArgument(QString::number(diffusionOptions->seed()));
    }

    QString commandLine;
    for (auto argument : arguments) {
        commandLine.append(argument);
        commandLine.append(" ");
    }
    qDebug()<<arguments;
    qDebug()<<commandLine;
    startProcess();


}
