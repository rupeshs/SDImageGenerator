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

#ifndef PYTHONENVVALIDATOR_H
#define PYTHONENVVALIDATOR_H

#include <QObject>
#include "diffusionenvironment.h"
#include "myprocess.h"
#include <QDebug>
#include <QStringList>
#include <QRegExp>

static QRegExp rxPackageLong("(\\w+-\\w+)");
static QRegExp rxPackage("(\\w+)");

class PythonEnvValidator : public QObject
{
    Q_OBJECT
public:
    explicit PythonEnvValidator(QObject *parent = nullptr,DiffusionEnvironment *diffEnv=nullptr);
    void validatePackages();

signals:
    void packageValidationCompleted(int exitCode,bool isPackagesReady);

public slots:
    void readProcessOutput(QByteArray);
    void processFinished(int, QProcess::ExitStatus);
    void processError(QProcess::ProcessError error);

private:
     MyProcess *pipProcess;
     DiffusionEnvironment *diffusionEnv;
     QStringList packages ;
     QStringList pipListOutput;
     bool hasImportError;

     bool validate();

};

#endif // PYTHONENVVALIDATOR_H
