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
    diffusionSettings->setSaveOriginalImage(settings->value("Main/saveOriginalImage",DEFAULT_SAVE_ORIGINAL_IMAGE).toBool());
    diffusionSettings->setVariationAmount(settings->value("Main/variationAmount",DEFAULT_VARIATION_AMOUNT).toDouble());

    diffusionSettings->setUpscaler(settings->value("Upscaler/upscaler",DEFAULT_USE_UPSCALER).toBool());
    diffusionSettings->setUpscaleFactor(settings->value("Upscaler/upscaleFactor",DEFAULT_UPSCALE_FACTOR).toString());
    diffusionSettings->setUpscaleStrength(settings->value("Upscaler/upscaleStrength",DEFAULT_UPSCALE_STRENGTH).toDouble());

    diffusionSettings->setFaceRestoration(settings->value("FaceRestoration/faceRestoration",DEFAULT_USE_FACE_RESTORATION).toBool());
    diffusionSettings->setFaceRestorationStrength(settings->value("FaceRestoration/faceRestorationStrength",DEFAULT_FACE_RESTORATION_STRENGTH).toDouble());

    diffusionSettings->setImageToImage(settings->value("ImageToImage/imageToImage",DEFAULT_USE_IMG_TO_IMG).toBool());
    diffusionSettings->setFitImage(settings->value("ImageToImage/fitImage",DEFAULT_IMG_TO_IMG_FIT).toBool());
    diffusionSettings->setImageToImageStrength(settings->value("ImageToImage/imageToImageStrength",DEFAULT_IMG_TO_IMG_STRENGTH).toDouble());
    diffusionSettings->setInitImagePath(settings->value("ImageToImage/initImage",DEFAULT_INIT_IMAGE).toString());
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
    settings->setValue("saveOriginalImage", diffusionSettings->saveOriginalImage());
    settings->setValue("variationAmount", diffusionSettings->variationAmount());
    settings->endGroup();

    settings->beginGroup("Upscaler");
    settings->setValue("upscaler", diffusionSettings->upscaler());
    settings->setValue("upscaleFactor", diffusionSettings->upscaleFactor());
    settings->setValue("upscaleStrength", diffusionSettings->upscaleStrength());
    settings->endGroup();

    settings->beginGroup("FaceRestoration");
    settings->setValue("faceRestoration", diffusionSettings->faceRestoration());
    settings->setValue("faceRestorationStrength", diffusionSettings->faceRestorationStrength());
    settings->endGroup();

    settings->beginGroup("ImageToImage");
    settings->setValue("imageToImage", diffusionSettings->imageToImage());
    settings->setValue("fitImage", diffusionSettings->fitImage());
    settings->setValue("imageToImageStrength", diffusionSettings->imageToImageStrength());
    settings->setValue("initImage", diffusionSettings->initImagePath());
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
    diffusionSettings->setUpscaleStrength(DEFAULT_UPSCALE_STRENGTH);
    diffusionSettings->setFaceRestoration(DEFAULT_USE_FACE_RESTORATION);
    diffusionSettings->setFaceRestorationStrength(DEFAULT_FACE_RESTORATION_STRENGTH);
    diffusionSettings->setSaveOriginalImage(DEFAULT_SAVE_ORIGINAL_IMAGE);
    diffusionSettings->setImageToImage(DEFAULT_USE_IMG_TO_IMG);
    diffusionSettings->setFitImage(DEFAULT_IMG_TO_IMG_FIT);
    diffusionSettings->setImageToImageStrength(DEFAULT_IMG_TO_IMG_STRENGTH);
    diffusionSettings->setInitImagePath(DEFAULT_INIT_IMAGE);
    diffusionSettings->setVariationAmount(DEFAULT_VARIATION_AMOUNT);

}
