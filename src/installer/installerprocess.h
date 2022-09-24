#ifndef INSTALLERPROCESS_H
#define INSTALLERPROCESS_H

#include <QObject>
#include <QDebug>
#include <QRegExp>
#include "diffusionenvironment.h"
#include "myprocess.h"

static QRegExp rxDownloadPercentage("(\\d+)");

class InstallerProcess : public QObject
{
    Q_OBJECT
public:
    explicit InstallerProcess(QObject *parent = nullptr,DiffusionEnvironment *diffEnv=nullptr);
    void downloadStableDiffusionModel();
    void installCondaEnv();

    float getDownloadProgress() const;

public slots:
    void readProcessOutput(QByteArray);
    void processFinished(int, QProcess::ExitStatus);
    void processError(QProcess::ProcessError error);

signals:
     void gotConsoleLog(const QString &message);
     void installCompleted(int exitCode,bool isDownloader);

private:
    MyProcess *installerProc;
    DiffusionEnvironment *diffusionEnv;
    float downloadProgress;
    bool isDownloader;
};

#endif // INSTALLERPROCESS_H