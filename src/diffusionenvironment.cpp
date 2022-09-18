#include "diffusionenvironment.h"
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QGuiApplication>
#include "utils.h"

DiffusionEnvironment::DiffusionEnvironment(QObject *parent)
    : QObject{parent}
{

    envPathsFileName = Utils::pathAppend(qApp->applicationDirPath(),QString(ENV_PATHS_FILE));
    qDebug()<<envPathsFileName;

}

void DiffusionEnvironment::getEnvironment()
{
    QFile inputFile(envPathsFileName);
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          line.remove(QChar('"'), Qt::CaseInsensitive);
          paths.append(line.trimmed());
       }
       inputFile.close();
    }
    assert(paths.length()==3);
    setEnvironment();

}

const QString &DiffusionEnvironment::getCondaActivatePath() const
{
    return condaActivatePath;
}

const QString &DiffusionEnvironment::getPythonEnvPath() const
{
    return pythonEnvPath;
}

const QString &DiffusionEnvironment::getStableDiffusionPath() const
{
    return stableDiffusionPath;
}

const QString &DiffusionEnvironment::getStableDiffusionScript() const
{
    return stableDiffusionScript;
}
void DiffusionEnvironment::setCondaActivatePath(const QString &newCondaActivatePath)
{
    condaActivatePath = newCondaActivatePath;
}

void DiffusionEnvironment::setPythonEnvPath(const QString &newPythonEnvPath)
{
    pythonEnvPath = newPythonEnvPath;
}

void DiffusionEnvironment::setStableDiffusionPath(const QString &newStableDiffusionPath)
{
    stableDiffusionPath = newStableDiffusionPath;
}

void DiffusionEnvironment::setEnvironment()
{
  if (paths.length() > 0) {
      setCondaActivatePath( paths[0]);
      setPythonEnvPath( paths[1]);
      setStableDiffusionPath(paths[2]);
      stableDiffusionScript = Utils::pathAppend(stableDiffusionPath,QString(STABLE_DIFFUSION_OPTIM_FILE));
  }
}
