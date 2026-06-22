#pragma once

#ifndef VNOVEL_BACKGROUND_H
#define VNOVEL_BACKGROUND_H

#include <vnovel/engine.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct vn_background vn_background;

typedef enum vn_background_fit {
  VN_BACKGROUND_FIT_STRETCH,
  VN_BACKGROUND_FIT_CONTAIN,
  VN_BACKGROUND_FIT_COVER,
  VN_BACKGROUND_FIT_CENTER
} vn_background_fit;

vn_background *vn_background_load(vn_engine *engine, const char *path);

void vn_background_destroy(vn_background *background);

void vn_background_render(vn_background *background, vn_engine *engine);

void vn_background_set_fit(vn_background *background, vn_background_fit fit);

void vn_background_set_alpha(vn_background *background, float alpha);

float vn_background_width(const vn_background *background);

float vn_background_height(const vn_background *background);

#ifdef __cplusplus
}
#endif

#endif
