#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>
#include <QGuiApplication>
#include "defines.h"
#include "diffusionoptions.h"
#include "diffusionenvironment.h"

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = nullptr,
                      DiffusionOptions *diffusionOptions = nullptr,
                      DiffusionEnvironment *diffusionEnv = nullptr);
    void load();
    void save();
    void reset();

signals:

private:
    QSettings *settings;
    DiffusionOptions *diffusionSettings;
    DiffusionEnvironment *diffusionEnv;

};

#endif // SETTINGS_H
