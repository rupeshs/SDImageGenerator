#ifndef DEFINES_H
#define DEFINES_H

static const char* MINICONDA_ENV_DIRECTORY_WIN = "mc";
static const char* CURL_DIRECTORY_WIN = "curl/bin/curl.exe";
static const char* CONDA_ACTIVATE_SCRIPT_WIN = "condabin/activate.bat";
static const char* PYTHON_ENV_DIRECTORY = "env";
static const char* STABLE_DIFFUSION_DIRECTORY = "stablediffusion";
static const char* STABLE_DIFFUSION_MODEL_1_4 = "models/ldm/stable-diffusion-v1/model.ckpt";
static const char* STABLE_DIFFUSION_MODEL_1_4_URL = "https://www.googleapis.com/storage/v1/b/aai-blog-files/o/sd-v1-4.ckpt?alt=media";
static const char* STABLE_DIFFUSION_RESULTS_FOLDER_NAME = "results";
static const char* STABLE_DIFFUSION_DREAM = "scripts/dream.py";
static const char* STABLE_DIFFUSION_ENVIRONMENT_YAML ="environment.yaml";
static const double DEFAULT_SCALE = 7.5;
static const double DEFAULT_IMAGE_WIDTH = 512;
static const double DEFAULT_IMAGE_HEIGHT = 512;
static const double DEFAULT_NUMBER_OF_IMAGES = 1;
static const double DEFAULT_DDIM_STEPS = 50;
static const char* DEFAULT_SAMPLER = "plms";
static const double DEFAULT_SEED = 0;

static const char* LOG_FILE_NAME = "SdImageGenerator_logs.txt";

#endif // DEFINES_H

