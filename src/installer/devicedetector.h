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
