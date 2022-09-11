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

private:
    QString m_prompt;
    qreal m_scale;
    qreal m_imageWidth;
    qreal m_imageHeight;
    qreal m_numberOfImages;
    qreal m_ddimSteps;
    QString m_sampler;

};

#endif // DIFFUSIONOPTIONS_H
