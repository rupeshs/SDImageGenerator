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
#include <QUrl>
#include <QTimer>
#ifdef Q_OS_WIN
#include <windows.h>
#endif

class TextToImageBackend : public QObject,public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QString errorMessage MEMBER errorMsg NOTIFY gotErrorMessage)
    Q_PROPERTY(QString diffusionStatusMessage MEMBER diffusionStatusMsg NOTIFY statusChanged)
    Q_PROPERTY(QUrl samplesUrl MEMBER samplesPath NOTIFY samplesPathChanged)
    Q_PROPERTY(DiffusionOptions* options READ options CONSTANT)
    Q_PROPERTY(bool isProcessing MEMBER isProcessing NOTIFY isProcessingChanged)

public:
    explicit TextToImageBackend(QObject *parent = nullptr);

    DiffusionOptions *options() const;
    void setOptions(DiffusionOptions *newOptions);
    void classBegin();
    void componentComplete();

    bool getIsProcessing() const;
    void setIsProcessing(bool newIsProcessing);

public slots:
    void generateImage();
    void stopProcessing();
    void diffusionConsoleLine(QString);
    void showErrorDlg(const QString &errorMesg);
    void saveSettings();
    void loadSettings();
    void resetSettings();
    void stableDiffusionFinished();
    void openOutputFolder();
    void setOutputFolder(QUrl url);
    void installEnvironment();

signals:
    void showMessageBox();
    void gotErrorMessage();
    void statusChanged();
    void samplesPathChanged();
    void initControls(DiffusionOptions* options);
    void promptTextChanged();
    void guidanceScaleChanged();
    void imageWidthChanged();
    void setOutputDirectory(QString);
    void isProcessingChanged();

private:
    DiffusionProcess *stableDiffusion;
    DiffusionEnvironment *diffusionEnv;
    QString errorMsg;
    QString diffusionStatusMsg;
    QUrl samplesPath;
    DiffusionOptions *m_options;
    bool isProcessing;
    QSettings *settings;
    QString curOutputFolder;

    void initBackend();
    bool verifyEnvironment();
    void updateStatusMessage(const QString&);

};

#endif // TEXTTOIMAGEBACKEND_H
