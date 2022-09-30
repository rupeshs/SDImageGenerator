/*
    SDImageGenerator, Text to image generation AI app
    Copyright(C) 2022 Rupesh Sreeraman
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

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

QString DiffusionOptions::seed() const
{
    return m_seed;
}

void DiffusionOptions::setSeed(const QString & newSeed)
{
    m_seed = newSeed;
}

const QString &DiffusionOptions::saveDir() const
{
    return m_saveDir;
}

void DiffusionOptions::setSaveDir(const QString &newSaveDir)
{
    m_saveDir = newSaveDir;
}
