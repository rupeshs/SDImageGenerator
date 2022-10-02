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
    void addDreamScriptArgs(DiffusionOptions *diffusionOptions);
    void addSaveOriginalImageArg(bool saveImage);

};

#endif // DIFFUSIONPROCESS_H
