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

#include "texttoimagebackend.h"
#include <QApplication>
#include <QDateTime>
#include <QQmlApplicationEngine>

bool bInit = true;

void logMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
   Q_UNUSED(context);

   QString curTime = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
   QString log(curTime);

   switch (type)
   {
      case QtDebugMsg:
         log += Utils::getLogMessage("Debug",context.function,context.line,msg);
         break;
      case QtInfoMsg:
         log += Utils::getLogMessage("Info",context.function,context.line,msg);
         break;
      case QtWarningMsg:
         log += Utils::getLogMessage("Warning",context.function,context.line,msg);
         break;
      case QtCriticalMsg:
         log += Utils::getLogMessage("Critical",context.function,context.line,msg);
         break;
      case QtFatalMsg:
         log += Utils::getLogMessage("Fatal",context.function,context.line,msg);
         break;
   }

   QFile outFile(Utils::pathAppend(qApp->applicationDirPath(),LOG_FILE_NAME));
   if (bInit) {
       outFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
       bInit = false;
   } else {
       outFile.open(QIODevice::WriteOnly | QIODevice::Append);
   }

   QTextStream textStream(&outFile);
   textStream << log << endl;
}

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    //qInstallMessageHandler(logMessageHandler);
    QApplication app(argc, argv);

    qmlRegisterType<TextToImageBackend>("StableDiffusion",1,0,"TextToImageBackend");
    QQmlApplicationEngine engine;

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
