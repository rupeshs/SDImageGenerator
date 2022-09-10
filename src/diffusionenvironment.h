#ifndef DIFFUSIONENVIRONMENT_H
#define DIFFUSIONENVIRONMENT_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include "defines.h"

class DiffusionEnvironment : public QObject
{
    Q_OBJECT
public:
    explicit DiffusionEnvironment(QObject *parent = nullptr);
    void getEnvironment();

    const QString &getCondaActivatePath() const;
    const QString &getPythonEnvPath() const;
    const QString &getStableDiffusionPath() const;
    const QString &getStableDiffusionScript() const;

signals:

private:
    QString envPathsFileName;
    QStringList paths;
    QString condaActivatePath;
    QString pythonEnvPath;
    QString stableDiffusionPath;
    QString stableDiffusionScript;

    void setCondaActivatePath(const QString &newCondaActivatePath);
    void setPythonEnvPath(const QString &newPythonEnvPath);
    void setStableDiffusionPath(const QString &newStableDiffusionPath);
    void setEnvironment();

};

#endif // DIFFUSIONENVIRONMENT_H
