/*
    SDImageGenerator, Text to image generation AI app
    Copyright(C) 2022 Rupesh Sreeraman
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "diffusionenvironment.h"


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

const QString &DiffusionEnvironment::getCurlPath() const
{
    return curlPath;
}

const QString &DiffusionEnvironment::getStableDiffusionModelUrl() const
{
    return stableDiffusionModelUrl;
}

const QString &DiffusionEnvironment::getDefaultOutDir() const
{
    return defaultOutDir;
}

const QString &DiffusionEnvironment::getGfpGanModelPath() const
{
    return gfpGanModelPath;
}

const QString &DiffusionEnvironment::getGfpGanModelUrl() const
{
    return gfpGanModelUrl;
}

const QString &DiffusionEnvironment::getLibsTestScriptPath() const
{
    return libsTestScriptPath;
}

const QString &DiffusionEnvironment::getCodeFormerModelPath() const
{
    return codeFormerModelPath;
}

const QString &DiffusionEnvironment::getCodeFormerModelUrl() const
{
    return codeFormerModelUrl;
}

const QString &DiffusionEnvironment::getTiConceptRootDirectoryPath() const
{
    return tiConceptRootDirectoryPath;
}

const QStringList &DiffusionEnvironment::getTiConceptStyles() const
{
    return tiConceptStyles;
}

const QString &DiffusionEnvironment::getDetectDeviceScriptPath() const
{
    return detectDeviceScriptPath;
}

void DiffusionEnvironment::setCondaActivatePath(const QString &newCondaActivatePath)
{
    condaActivatePath = newCondaActivatePath;
}

void DiffusionEnvironment::setStableDiffusionPath(const QString &newStableDiffusionPath)
{
    stableDiffusionPath = newStableDiffusionPath;
}

QStringList DiffusionEnvironment::getTiConcepFoldertNames(const QString &folderPath)
{
    QStringList tiFolderNames;
    QDir dir(folderPath);
    dir.setFilter(QDir::Dirs| QDir::NoSymLinks);
    QFileInfoList fileInfoList = dir.entryInfoList();

    foreach (const QFileInfo &fileInfo, fileInfoList) {
        if (fileInfo.isDir()){
            QString folderName = fileInfo.completeBaseName();
            QString learnedEmbedsPath = Utils::pathAppend(fileInfo.absoluteFilePath(),TEXTUAL_INVERSION_CONCEPT_FILE);
            if  (!folderName.isEmpty() && folderName !="." && folderName !="..") {
                if(QFile::exists(learnedEmbedsPath))
                    tiFolderNames << folderName;
            }
        }
    }

    return tiFolderNames;
}

void DiffusionEnvironment::setEnvironment()
{
#ifdef Q_OS_WIN
    QString condaPath = Utils::pathAppend(qApp->applicationDirPath(),QString(MINICONDA_ENV_DIRECTORY_WIN));
    condaActivatePath= Utils::pathAppend(condaPath,CONDA_ACTIVATE_SCRIPT_WIN);
    curlPath = Utils::pathAppend(condaPath,CURL_DIRECTORY_WIN);
#endif
    //TODO : Linux

    stableDiffusionPath = Utils::pathAppend(qApp->applicationDirPath(),QString(STABLE_DIFFUSION_DIRECTORY));
    stableDiffusionScriptPath = Utils::pathAppend(stableDiffusionPath,QString(STABLE_DIFFUSION_DREAM));
    stableDiffusionModelPath = Utils::pathAppend(stableDiffusionPath,QString(STABLE_DIFFUSION_MODEL_1_4));
    stableDiffusionModelUrl = STABLE_DIFFUSION_MODEL_1_4_URL;
    defaultOutDir = Utils::pathAppend(qApp->applicationDirPath(),STABLE_DIFFUSION_RESULTS_FOLDER_NAME);
    QString gfpGanPath = Utils::pathAppend(stableDiffusionPath,QString(GFP_GAN_PATH));
    gfpGanModelPath = Utils::pathAppend(gfpGanPath,QString(GFP_GAN_MODEL));
    gfpGanModelUrl = GFP_GAN_MODEL_URL;
    libsTestScriptPath = Utils::pathAppend(stableDiffusionPath,QString(LIBS_TEST_SCRIPT));
    codeFormerModelPath = Utils::pathAppend(stableDiffusionPath,QString(CODE_FORMER_MODEL_PATH));
    codeFormerModelUrl = CODE_FORMER_MODEL_URL;
    tiConceptRootDirectoryPath = Utils::pathAppend(qApp->applicationDirPath(),QString(TEXTUAL_INVERSION_DIR));
    tiConceptStyles = getTiConcepFoldertNames(tiConceptRootDirectoryPath);
    detectDeviceScriptPath = Utils::pathAppend(stableDiffusionPath,QString(DETECT_DEVICE_SCRIPT));

}
