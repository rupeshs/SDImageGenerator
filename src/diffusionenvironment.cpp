#include "diffusionenvironment.h"
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QGuiApplication>


DiffusionEnvironment::DiffusionEnvironment(QObject *parent)
    : QObject{parent}
{

}

void DiffusionEnvironment::getEnvironment()
{
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
    return stableDiffusionScriptPath;
}

const QString &DiffusionEnvironment::getStableDiffusionModelPath() const
{
    return stableDiffusionModelPath;
}

void DiffusionEnvironment::setStableDiffusionModelPath(const QString &newStableDiffusionModelPath)
{
    stableDiffusionModelPath = newStableDiffusionModelPath;
}

const QString &DiffusionEnvironment::getCurlPath() const
{
    return curlPath;
}

const QString &DiffusionEnvironment::getStableDiffusionModelUrl() const
{
    return stableDiffusionModelUrl;
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
#ifdef Q_OS_WIN
    QString condaPath = Utils::pathAppend(qApp->applicationDirPath(),QString(MINICONDA_ENV_DIRECTORY_WIN));
    condaActivatePath= Utils::pathAppend(condaPath,CONDA_ACTIVATE_SCRIPT_WIN);
    curlPath = Utils::pathAppend(condaPath,CURL_DIRECTORY_WIN);
#endif
    //TODO : Linux

    pythonEnvPath = Utils::pathAppend(qApp->applicationDirPath(),QString(PYTHON_ENV_DIRECTORY));
    stableDiffusionPath = Utils::pathAppend(qApp->applicationDirPath(),QString(STABLE_DIFFUSION_DIRECTORY));
    stableDiffusionScriptPath = Utils::pathAppend(stableDiffusionPath,QString(STABLE_DIFFUSION_DREAM));
    stableDiffusionModelPath = Utils::pathAppend(stableDiffusionPath,QString(STABLE_DIFFUSION_MODEL_1_4));
    stableDiffusionModelUrl = STABLE_DIFFUSION_MODEL_1_4_URL;

}
