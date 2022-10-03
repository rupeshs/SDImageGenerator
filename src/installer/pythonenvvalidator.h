#ifndef PYTHONENVVALIDATOR_H
#define PYTHONENVVALIDATOR_H

#include <QObject>
#include "diffusionenvironment.h"
#include "myprocess.h"
#include <QDebug>
#include <QStringList>
#include <QRegExp>

static QRegExp rxPackageLong("(\\w+-\\w+)");
static QRegExp rxPackage("(\\w+)");
static QRegExp rxDeviceInfo("DEVICE,(.*)");

class PythonEnvValidator : public QObject
{
    Q_OBJECT
public:
    explicit PythonEnvValidator(QObject *parent = nullptr,DiffusionEnvironment *diffEnv=nullptr);
    void validatePackages();

signals:
    void packageValidationCompleted(int exitCode,bool isPackagesReady);
    void gotDeviceInfo(const QString &deviceInfo);

public slots:
    void readProcessOutput(QByteArray);
    void processFinished(int, QProcess::ExitStatus);
    void processError(QProcess::ProcessError error);

private:
     MyProcess *pipProcess;
     DiffusionEnvironment *diffusionEnv;
     QStringList packages ;
     QStringList pipListOutput;
     bool hasImportError;

     bool validate();

};

#endif // PYTHONENVVALIDATOR_H
