#include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include "texttoimagebackend.h"
#include <QQuickStyle>
#include <QDateTime>



void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
   Q_UNUSED(context);

   QString dt = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
   QString txt = QString("[%1] ").arg(dt);

   switch (type)
   {
      case QtDebugMsg:
         txt += QString("[Debug] %1 %2").arg(msg,context.function);
         break;
      case QtWarningMsg:
         txt += QString("[Warning] %1").arg(msg);
         break;
      case QtCriticalMsg:
         txt += QString("[Critical] %1").arg(msg);
         break;
      case QtFatalMsg:
         txt += QString("[Fatal] %1").arg(msg);
         abort();
         break;
   }

   QFile outFile("SdImageGenerator_logs.txt");
   outFile.open(QIODevice::WriteOnly | QIODevice::Append);

   QTextStream textStream(&outFile);
   textStream << txt << endl;
}

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QApplication app(argc, argv);

    qmlRegisterType<TextToImageBackend>("StableDiffusion",1,0,"TextToImageBackend");
    QQmlApplicationEngine engine;
    qInstallMessageHandler(customMessageHandler);

    //qputenv("QT_QUICK_CONTROLS_STYLE", QByteArray("Universal"));
    //qputenv("QT_QUICK_CONTROLS_UNIVERSAL_THEME", QByteArray("Dark"));

    //QQuickStyle::setStyle("Imagine");
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);


    return app.exec();
}
