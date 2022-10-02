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

#include "diffusionenvironmentstatus.h"

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

bool DiffusionEnvironmentStatus::isGfpGanModelReady() const
{
    return m_isGfpGanModelReady;
}

void DiffusionEnvironmentStatus::setIsGfpGanModelReady(bool newIsGfpGanModelReady)
{
    m_isGfpGanModelReady = newIsGfpGanModelReady;
}
