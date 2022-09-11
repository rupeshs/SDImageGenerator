#ifndef DIFFUSIONENVVALIDATOR_H
#define DIFFUSIONENVVALIDATOR_H

#include <QObject>
#include <diffusionenvironment.h>
#include "errors.h"
#include "utils.h"

class DiffusionEnvValidator : public QObject
{
    Q_OBJECT
public:
    explicit DiffusionEnvValidator(QObject *parent = nullptr, DiffusionEnvironment *diffusionEnv = nullptr);
    EnvStatus Validate();

signals:

private:
    DiffusionEnvironment *diffusionEnv;

};

#endif // DIFFUSIONENVVALIDATOR_H
