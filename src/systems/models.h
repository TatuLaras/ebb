#ifndef _MODELS
#define _MODELS

#include "handles.h"
#include <raylib.h>

void sysinit_models(void);
void sysdeinit_models(void);
void models_set_asset_directory(const char *dir);
Model *models_get(ModelHandle handle);

#endif
