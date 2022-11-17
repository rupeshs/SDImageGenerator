#ifndef MODELSFINDER_H
#define MODELSFINDER_H

#include <QObject>
#include "diffusionenvironment.h"
#include "myprocess.h"
#include <QDebug>
#include <QStringList>


class ModelsFinder : public QObject
{
    Q_OBJECT
public:
    explicit ModelsFinder(QObject *parent = nullptr,DiffusionEnvironment *diffEnv=nullptr);
    void find();

public slots:
    void readProcessOutput(QByteArray);
    void processFinished(int, QProcess::ExitStatus);
    void processError(QProcess::ProcessError error);

signals:
   void gotModelsList(QStringList modelNames);

private:
     MyProcess *modelsFinderProcess;
     DiffusionEnvironment *diffusionEnv;
     QStringList stableDiffusionModels;
};

#endif // MODELSFINDER_H
