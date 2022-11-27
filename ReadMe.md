## SDImageGenerator
SDImageGenerator(SDIG) is a text-to-image generation AI desktop app  for Windows/Linux. It is still actively under development. The Linux version of the app is coming soon.

SDImageGenerator v2            |  SDIG output sample
:-------------------------:|:-------------------------:
![SDImageGenerator ](/images/SDImageGenerator_v2.PNG "SDImageGenerator 2.0") | ![SDImageGenerator output](/images/SDImageGenerator_v2_output.PNG "SDImageGenerator 2.0 output").



## Version 2.0.0, What's New? 
 - Works on CPU/GPU but CPU image generation is slow(Recommended to use GPU for faster image generation)
- The stable diffusion 1.5 model is used as default
- Added model switching (For advanced use)
- Supports dreambooth models (checkpoint files (.ckpt) supported)
- Added textual inversion support(Hugging Face's concepts library models supported)
- Added VAE(Variational autoencoder) support for fine details
- Added CodeFormer support
- Updated full precision mode with float32
- Added high-resolution duplication fix setting
- Image-to-image now supports k-diffusion samplers
- Added Image-to-image mask image support 
- Added cancel functionality
- Negative prompt support, use [ ] to use negative prompt
           E.g: `a cute [white] dog`
- Added Attention control support in prompt :
            Use "+" to increase attention - `a house, apple++ tree`
            Use "-" to decrease attention - `a house, apple- tree`
- Supports prompt-to-Prompt editing (Cross attention control)
            E.g:  In the below prompt cat will be replaced with a dog
                    `A cat.swap(dog) riding bicycle`
 - Supports prompt blending 
                    E.g: `car:0.30 boat:0.70 hybrid`

For older changelogs, please visit [changelog](changelog.md).
## Features 
- Simple interface
- Portable package, simply extract and run the app
- Image viewer for generated images
- Application logs
- Configurable image generation settings

## System Requirements:
- Windows 10/11 64-bit
- Recent Nvidia GPU, preferably RTX ( 4 GB or more VRAM memory)
- 12 GB RAM (16+ recommended)
- ~11 GB disk space after installation (on SSD for best performance)

## Development Dependencies 
- Qt 5.15.2

## Coding Style
If you want to contribute please follow the coding style
- [Qt Coding Style](https://wiki.qt.io/Qt_Coding_Style)

## Python Part

[Stable Diffusion fork](https://github.com/rupeshs/InvokeAI/tree/sdig)

## Download SDImageGenerator
[Download latest release ](https://rupeshsreeraman.itch.io/sdimagegenerator)