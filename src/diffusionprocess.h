#ifndef DIFFUSIONPROCESS_H
#define DIFFUSIONPROCESS_H

#include <QObject>
#include <QProcess>
#include "diffusionenvironment.h"
#include "diffusionoptions.h"

class DiffusionProcess: public QObject
{
    Q_OBJECT

public:
    explicit DiffusionProcess(QObject *parent = nullptr,DiffusionEnvironment *diffEnv=nullptr);

    void setProgram(QString programPath){exePath=programPath;}
    void addArgument(QString arg){arguments.append(arg);}
    void clearArguments(){arguments.clear();}
    void startProcess();
    void stopProcess();
    void generateImages(DiffusionOptions *diffusionOptions);

    int getStyleStrength() const;
    void setStyleStrength(int newStyleStrength);

public slots:
    void readProcessOutput();
    void processFinished(int, QProcess::ExitStatus);
    void processError(QProcess::ProcessError error);

signals:
    void diffusionConsoleLine(QString consoleLine);
    void diffusionFinished();

private:
    QProcess *stableDiffusionProcess;
    QStringList arguments;
    QString exePath;

    int styleStrength;

};

#endif // DIFFUSIONPROCESS_H
