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

#ifndef DIFFUSIONENVIRONMENT_H
#define DIFFUSIONENVIRONMENT_H

#include "defines.h"
#include "utils.h"
#include <QFile>
#include <QObject>
#include <QTextStream>
#include <QGuiApplication>


class DiffusionEnvironment : public QObject
{
    Q_OBJECT
public:
    explicit DiffusionEnvironment(QObject *parent = nullptr);

    void getEnvironment();

    const QString &getCondaActivatePath() const;
    const QString &getStableDiffusionPath() const;
    const QString &getStableDiffusionScript() const;
    const QString &getStableDiffusionModelPath() const;
    const QString &getCurlPath() const;
    const QString &getStableDiffusionModelUrl() const;
    const QString &getDefaultOutDir() const;
    const QString &getGfpGanModelPath() const;
    const QString &getGfpGanModelUrl() const;
    const QString &getLibsTestScriptPath() const;

signals:

private:
    QString envPathsFileName;
    QStringList paths;
    QString condaActivatePath;
    QString stableDiffusionPath;
    QString stableDiffusionScriptPath;
    QString stableDiffusionModelPath;
    QString curlPath;
    QString stableDiffusionModelUrl;
    QString defaultOutDir;
    QString gfpGanModelPath;
    QString gfpGanModelUrl;
    QString libsTestScriptPath;

    void setCondaActivatePath(const QString &newCondaActivatePath);
    void setPythonEnvPath(const QString &newPythonEnvPath);
    void setStableDiffusionPath(const QString &newStableDiffusionPath);
    void setEnvironment();

};

#endif // DIFFUSIONENVIRONMENT_H
