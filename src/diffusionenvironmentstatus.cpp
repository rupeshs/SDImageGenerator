#include "diffusionenvironmentstatus.h"

bool DiffusionEnvironmentStatus::isCondaReady() const
{
    return m_isCondaReady;
}

void DiffusionEnvironmentStatus::setIsCondaReady(bool newIsCondaReady)
{
    m_isCondaReady = newIsCondaReady;
}

bool DiffusionEnvironmentStatus::isPythonEnvReady() const
{
    return m_isPythonEnvReady;
}

void DiffusionEnvironmentStatus::setIsPythonEnvReady(bool newIsPythonEnvReady)
{
    m_isPythonEnvReady = newIsPythonEnvReady;
}

bool DiffusionEnvironmentStatus::isStableDiffusionModelReady() const
{
    return m_isStableDiffusionModelReady;
}

void DiffusionEnvironmentStatus::setIsStableDiffusionModelReady(bool newIsStableDiffusionModelReady)
{
    m_isStableDiffusionModelReady = newIsStableDiffusionModelReady;
}
