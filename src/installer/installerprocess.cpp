#include "installerprocess.h"

InstallerProcess::InstallerProcess(QObject *parent,DiffusionEnvironment *diffEnv)
    : QObject{parent}
{
    installerProc = new MyProcess(parent);
    connect(installerProc,SIGNAL(lineAvailable(QByteArray)),this,SLOT(readProcessOutput(QByteArray)));
    connect(installerProc,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(processFinished(int,QProcess::ExitStatus)));
    downloadProgress = 0.0;
    isDownloader = false;
    diffusionEnv = diffEnv;
}

void InstallerProcess::downloadStableDiffusionModel()
{
    installerProc->clearArguments();
    installerProc->addArgument(diffusionEnv->getCurlPath());
    installerProc->addArgument("-k");
    installerProc->addArgument(diffusionEnv->getStableDiffusionModelUrl());
    installerProc->addArgument("-o");
    installerProc->addArgument(diffusionEnv->getStableDiffusionModelPath());
    installerProc->start();
    isDownloader = true;
}

void InstallerProcess::installCondaEnv()
{

    installerProc->clearArguments();
    installerProc->addArgument(diffusionEnv->getCondaActivatePath());
    installerProc->addArgument("&&");
    installerProc->addArgument("conda");
    installerProc->addArgument("env");
    installerProc->addArgument("create");
    //installerProc->addArgument(QString("--prefix=") + diffusionEnv->getPythonEnvPath());
    QString envFile = Utils::pathAppend(diffusionEnv->getStableDiffusionPath(),STABLE_DIFFUSION_ENVIRONMENT_YAML);
    installerProc->addArgument("-f");
    installerProc->addArgument(envFile);
    qDebug()<<installerProc->arguments();
    installerProc->setWorkingDirectory(diffusionEnv->getStableDiffusionPath());
    installerProc->start();

    //conda env create --prefix="%~dp0\env" -f "%~dp0\stable-diffusion\environment.yaml"

}

void InstallerProcess::installPipPackages()
{
    installerProc->clearArguments();
    installerProc->addArgument(diffusionEnv->getCondaActivatePath());
    installerProc->addArgument("&&");
    installerProc->addArgument("conda");
    installerProc->addArgument("activate");
    installerProc->addArgument("sdimgenv");
    installerProc->addArgument("&&");
    addPackageArgs("./src/clip");
    addPackageArgs("./src/gfpgan");
    addPackageArgs("./src/k-diffusion");
    addPackageArgs("./src/taming-transformers");
    addPackageArgs("./src/clipseg");
    addPackageArgs("./src/realesrgan");
    addPackageArgs(".");
    installerProc->setWorkingDirectory(diffusionEnv->getStableDiffusionPath());
    installerProc->start();


}

void InstallerProcess::downloadGfpganModel()
{
    installerProc->clearArguments();
    installerProc->addArgument(diffusionEnv->getCurlPath());
    installerProc->addArgument("-L");
    installerProc->addArgument("-k");
    installerProc->addArgument(diffusionEnv->getGfpGanModelUrl());
    installerProc->addArgument("-o");
    installerProc->addArgument(diffusionEnv->getGfpGanModelPath());
    qDebug()<<installerProc->arguments();
    installerProc->start();
    isDownloader = true;
}

void InstallerProcess::downloadCodeFormerModel()
{
    installerProc->clearArguments();
    installerProc->addArgument(diffusionEnv->getCurlPath());
    installerProc->addArgument("-L");
    installerProc->addArgument("-k");
    installerProc->addArgument(diffusionEnv->getCodeFormerModelUrl());
    installerProc->addArgument("-o");
    installerProc->addArgument(diffusionEnv->getCodeFormerModelPath());
    qDebug()<<installerProc->arguments();
    installerProc->start();
    isDownloader = true;
}

void InstallerProcess::readProcessOutput(QByteArray line)
{
    QString consoleLine(line);

    if (!line.isEmpty())
        emit gotConsoleLog(line);

    if(rxDownloadPercentage.indexIn(consoleLine)>-1){
       int percentage = rxDownloadPercentage.cap(1).toInt();
       downloadProgress = percentage / 100.0;
     }
}

void InstallerProcess::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
 qDebug() << "InstallerProcess: exit_code:" << exitCode << "exit_status:" << exitStatus;
 emit installCompleted(exitCode,isDownloader);
}

void InstallerProcess::processError(QProcess::ProcessError error)
{
    qDebug() << "Installer ProcessError:" << error;
    if (error == QProcess::FailedToStart) {
        qDebug()<<"Installer process failed to start";
    }
}

float InstallerProcess::getDownloadProgress() const
{
    return downloadProgress;
}
void InstallerProcess::stopProcess()
{
    if (installerProc) {
        installerProc->terminate();
        if (!installerProc->waitForFinished(2000)) {
            installerProc->kill();
            installerProc->waitForFinished();
        }
    }
}

void InstallerProcess::addPackageArgs(const QString &packagePath)
{
    installerProc->addArgument("pip");
    installerProc->addArgument("install");
    installerProc->addArgument("--no-input");
    installerProc->addArgument("-e");
    installerProc->addArgument(packagePath);
}
