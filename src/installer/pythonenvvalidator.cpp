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

    if(rxDeviceInfo.indexIn(consoleLine)>-1){
       QString device = rxDeviceInfo.cap(1);
       emit gotDeviceInfo(device);
    }

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
