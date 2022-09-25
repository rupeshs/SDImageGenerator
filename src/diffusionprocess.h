#ifndef DIFFUSIONPROCESS_H
#define DIFFUSIONPROCESS_H

#include <QObject>
#include <QDir>
#include "diffusionenvironment.h"
#include "diffusionoptions.h"
#include "myprocess.h"
#include "utils.h"
#include <QRegExp>

static QRegExp rxOutputFolder("Writing files to directory: \"(.*)\"");
enum class StableDiffusionStatus { NotStarted, Starting, Ready, Processing };

class DiffusionProcess: public QObject
{
    Q_OBJECT

public:
    explicit DiffusionProcess(QObject *parent = nullptr,DiffusionEnvironment *diffEnv=nullptr);

    void addArgument(QString arg){ dreamProcess->addArgument(arg); }
    void addPromptArguments(QString arg){promptArguments.append(arg);}
    void clearPromptArguments(){promptArguments.clear();}
    void clearArguments(){arguments.clear();}
    void startProcess();
    void stopProcess();
    void generateImages(DiffusionOptions *diffusionOptions);

    const QString &getPromptCommand() const;
    StableDiffusionStatus getStatus() const;

    const QUrl &getSamplesPath() const;

public slots:
    void readProcessOutput(QByteArray);
    void processFinished(int, QProcess::ExitStatus);
    void processError(QProcess::ProcessError error);
    void writeCommand(const QString & command);

signals:
    void generatingImages();
    void imagesGenerated();
    void diffusionFinished();
    void gotConsoleLog(const QString &message);
    void cudaMemoryError();

private:
    MyProcess *dreamProcess;
    QStringList arguments;
    QStringList promptArguments;
    QString promptCommand;
    StableDiffusionStatus status;
    QUrl samplesPath;

    void startDreaming();
    void setStatus(StableDiffusionStatus newStatus);

};

#endif // DIFFUSIONPROCESS_H
