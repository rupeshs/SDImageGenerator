#ifndef DEFINES_H
#define DEFINES_H

static const char* ENV_PATHS_FILE = "envpaths.txt";
static const char* STABLE_DIFFUSION_OPTIM_FILE = "optimizedSD/optimized_txt2img.py";
static const char* STABLE_DIFFUSION_DEFAULT_OUTPUT_PATH = "outputs/txt2img-samples";

static const double DEFAULT_SCALE = 7.5;
static const double DEFAULT_IMAGE_WIDTH = 512;
static const double DEFAULT_IMAGE_HEIGHT = 512;
static const double DEFAULT_NUMBER_OF_IMAGES = 1;
static const double DEFAULT_DDIM_STEPS = 50;
static const char* DEFAULT_SAMPLER = "plms";
static const double DEFAULT_SEED = 0;
#endif // DEFINES_H
