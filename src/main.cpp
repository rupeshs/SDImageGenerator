#include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include "texttoimagebackend.h"
#include <QQuickStyle>

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QApplication app(argc, argv);

    qmlRegisterType<TextToImageBackend>("StableDiffusion",1,0,"TextToImageBackend");
    QQmlApplicationEngine engine;

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
