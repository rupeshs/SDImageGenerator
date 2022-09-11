#include "diffusionoptions.h"

const QString &DiffusionOptions::prompt() const
{
    return m_prompt;
}

void DiffusionOptions::setPrompt(const QString &newPrompt)
{
    m_prompt = newPrompt;
}

qreal DiffusionOptions::scale() const
{
    return m_scale;
}

void DiffusionOptions::setScale(qreal newScale)
{
    m_scale = newScale;
}

qreal DiffusionOptions::imageWidth() const
{
    return m_imageWidth;
}

void DiffusionOptions::setImageWidth(qreal newImageWidth)
{
    m_imageWidth = newImageWidth;
}

qreal DiffusionOptions::imageHeight() const
{
    return m_imageHeight;
}

void DiffusionOptions::setImageHeight(qreal newImageHeight)
{
    m_imageHeight = newImageHeight;
}

qreal DiffusionOptions::numberOfImages() const
{
    return m_numberOfImages;
}

void DiffusionOptions::setNumberOfImages(qreal newNumberOfImages)
{
    m_numberOfImages = newNumberOfImages;
}

qreal DiffusionOptions::ddimSteps() const
{
    return m_ddimSteps;
}

void DiffusionOptions::setDdimSteps(qreal newDdimSteps)
{
    m_ddimSteps = newDdimSteps;
}

const QString &DiffusionOptions::sampler() const
{
    return m_sampler;
}

void DiffusionOptions::setSampler(const QString &newSampler)
{
    m_sampler = newSampler;
}

qreal DiffusionOptions::seed() const
{
    return m_seed;
}

void DiffusionOptions::setSeed(qreal newSeed)
{
    m_seed = newSeed;
}
