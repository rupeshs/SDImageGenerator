#include "devicedetector.h"

DeviceDetector::DeviceDetector(QObject *parent,DiffusionEnvironment *diffEnv)
    : QObject{parent}
{
    deviceDetectorProcess = new MyProcess(parent);
    connect(deviceDetectorProcess,SIGNAL(lineAvailable(QByteArray)),this,SLOT(readProcessOutput(QByteArray)));
    connect(deviceDetectorProcess,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(processFinished(int,QProcess::ExitStatus)));
    diffusionEnv = diffEnv;

}

void DeviceDetector::detect()
{
    deviceDetectorProcess->clearArguments();
    deviceDetectorProcess->addArgument(diffusionEnv->getCondaActivatePath());
    deviceDetectorProcess->addArgument("&&");
    deviceDetectorProcess->addArgument("conda");
    deviceDetectorProcess->addArgument("activate");
    deviceDetectorProcess->addArgument("sdimgenv");
    deviceDetectorProcess->addArgument("&&");
    deviceDetectorProcess->addArgument("python");
    deviceDetectorProcess->addArgument(diffusionEnv->getDetectDeviceScriptPath());

    deviceDetectorProcess->start();
}
void DeviceDetector::readProcessOutput(QByteArray line)
{
    QString consoleLine(line);
    if(rxDeviceInfo.indexIn(consoleLine)>-1){
       QString device = rxDeviceInfo.cap(1);
       emit gotDeviceInfo(device);
    }
}

void DeviceDetector::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "DeviceDetector: exitCode:" << exitCode << "exitStatus:" << exitStatus;
}

void DeviceDetector::processError(QProcess::ProcessError error)
{
    qDebug() << "DeviceDetector ProcessError:" << error;
    if (error == QProcess::FailedToStart) {
        qDebug()<<"DeviceDetector failed to start";
    }
}
