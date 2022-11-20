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

#ifndef DEVICEDETECTOR_H
#define DEVICEDETECTOR_H

#include <QObject>
#include "diffusionenvironment.h"
#include "myprocess.h"
#include <QDebug>
#include <QStringList>
#include <QRegExp>

static QRegExp rxDeviceInfo("DEVICE,(.*)");

class DeviceDetector : public QObject
{
    Q_OBJECT
public:
    explicit DeviceDetector(QObject *parent = nullptr,DiffusionEnvironment *diffEnv=nullptr);
     void detect();

signals:
    void gotDeviceInfo(const QString &deviceInfo);

public slots:
    void readProcessOutput(QByteArray);
    void processFinished(int, QProcess::ExitStatus);
    void processError(QProcess::ProcessError error);


private:
     MyProcess *deviceDetectorProcess;
     DiffusionEnvironment *diffusionEnv;

};

#endif // DEVICEDETECTOR_H
