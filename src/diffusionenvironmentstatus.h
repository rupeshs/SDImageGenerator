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

#ifndef DIFFUSIONENVIRONMENTSTATUS_H
#define DIFFUSIONENVIRONMENTSTATUS_H

#include <QObject>
#include <qqml.h>

class DiffusionEnvironmentStatus : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isPythonEnvReady READ isPythonEnvReady WRITE setIsPythonEnvReady CONSTANT)
    Q_PROPERTY(bool isStableDiffusionModelReady READ isStableDiffusionModelReady WRITE setIsStableDiffusionModelReady CONSTANT)
    QML_ELEMENT

public:
    bool isPythonEnvReady() const;
    void setIsPythonEnvReady(bool newIsPythonEnvReady);

    bool isStableDiffusionModelReady() const;
    void setIsStableDiffusionModelReady(bool newIsStableDiffusionModelReady);

private:
     bool m_isPythonEnvReady;
     bool m_isStableDiffusionModelReady;
};

#endif // DIFFUSIONENVIRONMENTSTATUS_H
