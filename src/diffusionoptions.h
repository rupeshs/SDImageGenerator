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

#ifndef DIFFUSIONOPTIONS_H
#define DIFFUSIONOPTIONS_H

#include <QObject>
#include <qqml.h>

class DiffusionOptions : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString prompt READ prompt WRITE setPrompt CONSTANT)
    Q_PROPERTY(qreal scale READ scale WRITE setScale CONSTANT)
    Q_PROPERTY(qreal imageWidth READ imageWidth WRITE setImageWidth CONSTANT)
    Q_PROPERTY(qreal imageHeight READ imageHeight WRITE setImageHeight CONSTANT)
    Q_PROPERTY(qreal numberOfImages READ numberOfImages WRITE setNumberOfImages CONSTANT)
    Q_PROPERTY(qreal ddimSteps READ ddimSteps WRITE setDdimSteps CONSTANT)
    Q_PROPERTY(QString sampler READ sampler WRITE setSampler CONSTANT)
    Q_PROPERTY(QString seed READ seed WRITE setSeed CONSTANT)
    Q_PROPERTY(QString saveDir READ saveDir WRITE setSaveDir CONSTANT)
    Q_PROPERTY(bool grid READ grid WRITE setGrid CONSTANT)
    Q_PROPERTY(bool seamless READ seamless WRITE setSeamless CONSTANT)
    QML_ELEMENT

public:
    using QObject::QObject;

    const QString &prompt() const;
    void setPrompt(const QString &newPrompt);

    qreal scale() const;
    void setScale(qreal newScale);

    qreal imageWidth() const;
    void setImageWidth(qreal newImageWidth);

    qreal imageHeight() const;
    void setImageHeight(qreal newImageHeight);

    qreal numberOfImages() const;
    void setNumberOfImages(qreal newNumberOfImages);

    qreal ddimSteps() const;
    void setDdimSteps(qreal newDdimSteps);

    const QString &sampler() const;
    void setSampler(const QString &newSampler);

    QString seed() const;
    void setSeed(const QString &newSeed);

    const QString &saveDir() const;
    void setSaveDir(const QString &newSaveDir);

    bool grid() const;
    void setGrid(bool newGrid);

    bool seamless() const;
    void setSeamless(bool newSeamless);

private:
    QString m_prompt;
    qreal m_scale;
    qreal m_imageWidth;
    qreal m_imageHeight;
    qreal m_numberOfImages;
    qreal m_ddimSteps;
    QString m_sampler;
    QString m_seed;
    QString m_saveDir;
    bool m_grid;
    bool m_seamless;

};

#endif // DIFFUSIONOPTIONS_H
