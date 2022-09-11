#ifndef TEXTTOIMAGEBACKEND_H
#define TEXTTOIMAGEBACKEND_H

#include <QObject>
#include <QSettings>
#include "diffusionprocess.h"
#include "diffusionenvironment.h"
#include "diffusionenvvalidator.h"
#include "diffusionoptions.h"
#include <QDebug>
#include <qqml.h>

class TextToImageBackend : public QObject,public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QString errorMessage MEMBER errorMsg NOTIFY gotErrorMessage)
    Q_PROPERTY(QString diffusionStatusMessage MEMBER diffusionStatusMsg NOTIFY statusChanged)
    Q_PROPERTY(QString outputPath MEMBER samplesPath NOTIFY loadImage)
    Q_PROPERTY(DiffusionOptions* options READ options CONSTANT)

public:
    explicit TextToImageBackend(QObject *parent = nullptr);

    DiffusionOptions *options() const;
    void setOptions(DiffusionOptions *newOptions);
    void classBegin();
    void componentComplete();


public slots:
    void generateImage();
    void stopProcessing();
    void diffusionConsoleLine(QString);
    void showErrorDlg(const QString &errorMesg);
    void saveSettings();
    void loadSettings();
    void resetSettings();
    void stableDiffusionFinished();

signals:
    void showMessageBox();
    void gotErrorMessage();
    void statusChanged();
    void loadImage();
    void initControls(DiffusionOptions* options);
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
    QSettings *settings;
    QString curOutputFolder;

    void initBackend();
    void verifyEnvironment();
    void updateStatusMessage(const QString&);


};

#endif // TEXTTOIMAGEBACKEND_H
