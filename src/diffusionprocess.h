#ifndef DIFFUSIONPROCESS_H
#define DIFFUSIONPROCESS_H

#include <QObject>
#include <QProcess>
#include "diffusionenvironment.h"
#include "diffusionoptions.h"
#include "myprocess.h"

class DiffusionProcess: public QObject
{
    Q_OBJECT

public:
    explicit DiffusionProcess(QObject *parent = nullptr,DiffusionEnvironment *diffEnv=nullptr);

    void setProgram(QString programPath){ exePath = programPath;}
    void addArgument(QString arg){ dreamProcess->addArgument(arg); }
    void addPromptArguments(QString arg){promptArguments.append(arg);}
    void clearPromptArguments(){promptArguments.clear();}
    void clearArguments(){arguments.clear();}
    void startProcess();
    void stopProcess();
    void generateImages(DiffusionOptions *diffusionOptions);

    int getStyleStrength() const;
    void setStyleStrength(int newStyleStrength);

    const QString &getPromptCommand() const;

public slots:
    void readProcessOutput(QByteArray);
    void processFinished(int, QProcess::ExitStatus);
    void processError(QProcess::ProcessError error);
    void writeCommand(const QString & command);


signals:
    void diffusionConsoleLine(QString consoleLine);
    void diffusionFinished();

private:
    QProcess *stableDiffusionProcess;
    MyProcess *dreamProcess;
    QStringList arguments;
    QStringList promptArguments;
    QString exePath;
    QString promptCommand;
    int styleStrength;

    void startDreaming();

};

#endif // DIFFUSIONPROCESS_H
