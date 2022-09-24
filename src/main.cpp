#include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include "texttoimagebackend.h"
#include <QQuickStyle>
#include <QDateTime>

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
   if(bInit){
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
    qInstallMessageHandler(logMessageHandler);
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
