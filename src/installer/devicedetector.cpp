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
