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

    //stableDiffusionProcess->setWorkingDirectory();
    qDebug()<<diffusionEnv->getCondaActivatePath();
    qDebug()<<diffusionEnv->getPythonEnvPath();
    qDebug()<<diffusionEnv->getStableDiffusionPath();
    stableDiffusionProcess->setWorkingDirectory(diffusionEnv->getStableDiffusionPath());

    exePath = diffusionEnv->getCondaActivatePath();
    addArgument("&&");
    addArgument("conda");
    addArgument("activate");
    addArgument( diffusionEnv->getPythonEnvPath());
    addArgument("&&");
    addArgument("python");
    addArgument(diffusionEnv->getStableDiffusionScript());

    addArgument("--prompt");
    addArgument("A fantasy landscape,trending on artstation");
    addArgument("--H");
    addArgument("512");
    addArgument("--W");
    addArgument("512");
    addArgument("--seed");
    addArgument("42");
    addArgument("--n_iter");
    addArgument(QString::number(1));
    addArgument("--n_samples");
    addArgument(QString::number(1));
    addArgument("--ddim_steps");
    addArgument(QString::number(10));

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
        if (!stableDiffusionProcess->waitForFinished(5000)) {
            stableDiffusionProcess->kill();
            stableDiffusionProcess->waitForFinished();
        }
    }
}
