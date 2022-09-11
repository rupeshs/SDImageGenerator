#ifndef TEXTTOIMAGEBACKEND_H
#define TEXTTOIMAGEBACKEND_H

#include <QObject>
#include "diffusionprocess.h"
#include "diffusionenvironment.h"
#include "diffusionenvvalidator.h"
#include "diffusionoptions.h"
#include <QDebug>
#include <qqml.h>


class TextToImageBackend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString errorMessage MEMBER errorMsg NOTIFY gotErrorMessage)
    Q_PROPERTY(QString diffusionStatusMessage MEMBER diffusionStatusMsg NOTIFY statusChanged)
    Q_PROPERTY(QString outputPath MEMBER samplesPath NOTIFY loadImage)

    Q_PROPERTY(DiffusionOptions* options READ options CONSTANT)


public:
    explicit TextToImageBackend(QObject *parent = nullptr);

    DiffusionOptions *options() const;
    void setOptions(DiffusionOptions *newOptions);
    static int const EXIT_CODE_REBOOT = -123456789;

public slots:
    void generateImage();
    void stopProcessing();
    void diffusionConsoleLine(QString);
    void showErrorDlg(const QString &errorMesg);

signals:
    void showMessageBox();
    void gotErrorMessage();
    void statusChanged();
    void loadImage();

    void promptTextChanged();
    void guidanceScaleChanged();
    void imageWidthChanged();


private:
    DiffusionProcess *stableDiffusion;
    DiffusionEnvironment *diffusionEnv;
    QString errorMsg;
    QString diffusionStatusMsg;
    QString samplesPath;
    DiffusionOptions *m_options;
    bool isProcessing;

    void verifyEnvironment();
    void updateStatusMessage(const QString&);



};

#endif // TEXTTOIMAGEBACKEND_H
