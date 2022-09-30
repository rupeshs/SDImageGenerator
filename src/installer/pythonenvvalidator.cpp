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
    pipProcess->addArgument("pip");
    pipProcess->addArgument("list");

    pipListOutput.clear();
    pipProcess->start();

}

void PythonEnvValidator::readProcessOutput(QByteArray line)
{
    QString consoleLine(line);
    if(rxPackage.indexIn(consoleLine)>-1){
       QString packageName = rxPackage.cap(1);
       pipListOutput.append(packageName);   
    }
    if(rxPackageLong.indexIn(consoleLine)>-1){
       QString packageName = rxPackageLong.cap(1);
       pipListOutput.append(packageName);
    }
}

void PythonEnvValidator::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "PythonEnvValidator: exitCode:" << exitCode << "exitStatus:" << exitStatus;
    bool packagesReady =  validate();
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
