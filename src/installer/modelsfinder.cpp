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
