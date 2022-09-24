#ifndef DIFFUSIONENVIRONMENTSTATUS_H
#define DIFFUSIONENVIRONMENTSTATUS_H

#include <QObject>
#include <qqml.h>

class DiffusionEnvironmentStatus : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isCondaReady READ isCondaReady WRITE setIsCondaReady CONSTANT)
    Q_PROPERTY(bool isPythonEnvReady READ isPythonEnvReady WRITE setIsPythonEnvReady CONSTANT)
    Q_PROPERTY(bool isStableDiffusionModelReady READ isStableDiffusionModelReady WRITE setIsStableDiffusionModelReady CONSTANT)
    QML_ELEMENT

public:
    bool isCondaReady() const;
    void setIsCondaReady(bool newIsCondaReady);

    bool isPythonEnvReady() const;
    void setIsPythonEnvReady(bool newIsPythonEnvReady);

    bool isStableDiffusionModelReady() const;
    void setIsStableDiffusionModelReady(bool newIsStableDiffusionModelReady);

private:
     bool m_isCondaReady;
     bool m_isPythonEnvReady;
     bool m_isStableDiffusionModelReady;
};

#endif // DIFFUSIONENVIRONMENTSTATUS_H
