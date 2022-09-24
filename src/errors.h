#ifndef ERRORS_H
#define ERRORS_H

enum class EnvStatus
  {
     CondaNotFound,
     PythonEnvNotFound,
     StableDiffusionNotFound,
     StableDiffusionModelNotFound,
     Ready
  };
#endif // ERRORS_H
