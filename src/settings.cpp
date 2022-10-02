#include "settings.h"

Settings::Settings(QObject *parent,DiffusionOptions *diffOptions, DiffusionEnvironment *diffEnv)
    : QObject{parent}
{
    settings = new QSettings(qApp->applicationDirPath()+"/sdimagegenerator.ini",QSettings::IniFormat,this);
    diffusionSettings = diffOptions;
    diffusionEnv = diffEnv;
}

void Settings::load()
{
    diffusionSettings->setPrompt(settings->value("Main/prompt","").toString());
    diffusionSettings->setScale(settings->value("Main/scale",DEFAULT_SCALE).toDouble());
    diffusionSettings->setImageWidth(settings->value("Main/imageWidth",DEFAULT_IMAGE_WIDTH).toDouble());
    diffusionSettings->setImageHeight(settings->value("Main/imageHeight",DEFAULT_IMAGE_HEIGHT).toDouble());
    diffusionSettings->setNumberOfImages(settings->value("Main/numberOfImages",DEFAULT_NUMBER_OF_IMAGES).toDouble());
    diffusionSettings->setDdimSteps(settings->value("Main/ddimSteps",DEFAULT_DDIM_STEPS).toDouble());
    diffusionSettings->setSampler(settings->value("Main/sampler",DEFAULT_SAMPLER).toString());
    diffusionSettings->setSaveDir(settings->value("Main/saveDir",diffusionEnv->getDefaultOutDir()).toString());
    QString seed = settings->value("Main/seed",DEFAULT_SEED).toString();
    diffusionSettings->setSeed(seed);
    diffusionSettings->setGrid(settings->value("Main/grid",DEFAULT_GRID).toBool());
    diffusionSettings->setSeamless(settings->value("Main/seamless",DEFAULT_SEAMLESS).toBool());
    diffusionSettings->setFullPrecision(settings->value("Main/fullPrecision",DEFAULT_FULL_PRECISION).toBool());
    diffusionSettings->setUpscaler(settings->value("Main/upscaler",DEFAULT_USE_UPSCALER).toBool());
    diffusionSettings->setUpscaleFactor(settings->value("Main/upscaleFactor",DEFAULT_UPSCALE_FACTOR).toString());
    diffusionSettings->setUpscaleStrength(settings->value("Main/upscaleStrength",DEFAULT_UPSCALE_STREGTH).toDouble());
    diffusionSettings->setFaceRestoration(settings->value("Main/faceRestoration",DEFAULT_USE_FACE_RESTORATION).toBool());
    diffusionSettings->setFaceRestorationStrength(settings->value("Main/faceRestorationStrength",DEFAULT_FACE_RESTORATION_STRENGTH).toDouble());
    diffusionSettings->setSaveOriginalImage(settings->value("Main/saveOriginalImage",DEFAULT_SAVE_ORIGINAL_IMAGE).toBool());

}

void Settings::save()
{
    settings->beginGroup("Main");
    settings->setValue("prompt", diffusionSettings->prompt());
    settings->setValue("scale", diffusionSettings->scale());
    settings->setValue("imageWidth", diffusionSettings->imageWidth());
    settings->setValue("imageHeight", diffusionSettings->imageHeight());
    settings->setValue("numberOfImages", diffusionSettings->numberOfImages());
    settings->setValue("ddimSteps", diffusionSettings->ddimSteps());
    settings->setValue("sampler", diffusionSettings->sampler());
    settings->setValue("seed", diffusionSettings->seed());
    settings->setValue("saveDir", diffusionSettings->saveDir());
    settings->setValue("grid", diffusionSettings->grid());
    settings->setValue("seamless", diffusionSettings->seamless());
    settings->setValue("fullPrecision", diffusionSettings->fullPrecision());
    settings->setValue("upscaler", diffusionSettings->upscaler());
    settings->setValue("upscaleFactor", diffusionSettings->upscaleFactor());
    settings->setValue("upscaleStrength", diffusionSettings->upscaleStrength());
    settings->setValue("faceRestoration", diffusionSettings->faceRestoration());
    settings->setValue("faceRestorationStrength", diffusionSettings->faceRestorationStrength());
    settings->setValue("saveOriginalImage", diffusionSettings->saveOriginalImage());
    settings->endGroup();
}

void Settings::reset()
{
    diffusionSettings->setScale(DEFAULT_SCALE);
    diffusionSettings->setImageWidth(DEFAULT_IMAGE_WIDTH);
    diffusionSettings->setImageHeight(DEFAULT_IMAGE_HEIGHT);
    diffusionSettings->setNumberOfImages(DEFAULT_NUMBER_OF_IMAGES);
    diffusionSettings->setDdimSteps(DEFAULT_DDIM_STEPS);
    diffusionSettings->setSampler(DEFAULT_SAMPLER);
    diffusionSettings->setSeed(DEFAULT_SEED);
    diffusionSettings->setSaveDir(diffusionEnv->getDefaultOutDir());
    diffusionSettings->setGrid(DEFAULT_GRID);
    diffusionSettings->setSeamless(DEFAULT_SEAMLESS);
    diffusionSettings->setFullPrecision(DEFAULT_FULL_PRECISION);
    diffusionSettings->setUpscaler(DEFAULT_USE_UPSCALER);
    diffusionSettings->setUpscaleFactor(DEFAULT_UPSCALE_FACTOR);
    diffusionSettings->setUpscaleStrength(DEFAULT_UPSCALE_STREGTH);
    diffusionSettings->setFaceRestoration(DEFAULT_USE_FACE_RESTORATION);
    diffusionSettings->setFaceRestorationStrength(DEFAULT_FACE_RESTORATION_STRENGTH);
    diffusionSettings->setSaveOriginalImage(DEFAULT_SAVE_ORIGINAL_IMAGE);

}
