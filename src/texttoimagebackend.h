#ifndef TEXTTOIMAGEBACKEND_H
#define TEXTTOIMAGEBACKEND_H

#include <QObject>
#include "diffusionprocess.h"
#include "diffusionenvironment.h"
#include "diffusionenvvalidator.h"
#include <QDebug>

class TextToImageBackend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString errorMessage MEMBER errorMsg NOTIFY gotErrorMessage)
    Q_PROPERTY(QString diffusionStatusMessage MEMBER diffusionStatusMsg NOTIFY statusChanged)
    Q_PROPERTY(QString outputPath MEMBER samplesPath NOTIFY loadImage)


public:
    explicit TextToImageBackend(QObject *parent = nullptr);


public slots:
    void generateImage();
    void stopProcessing();
    void diffusionConsoleLine(QString);

signals:
    void showMessageBox();
    void gotErrorMessage();
    void statusChanged();
    void loadImage();


private:
    DiffusionProcess *stableDiffusion;
    DiffusionEnvironment *diffusionEnv;
    QString errorMsg;
    QString diffusionStatusMsg;
    QString samplesPath;

    void verifyEnvironment();
    void updateStatusMessage(const QString&);

};

#endif // TEXTTOIMAGEBACKEND_H
