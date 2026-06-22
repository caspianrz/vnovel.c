#ifndef VNOVEL_ENGINE_H
#define VNOVEL_ENGINE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <SDL3/SDL.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct vn_engine vn_engine;

typedef struct vn_config {
  const char *title;
  uint32_t width;
  uint32_t height;
} vn_config;

typedef enum vn_result {
  VN_OK = 0,
  VN_ERROR = 1,
} vn_result;

/* Lifecycle */

vn_engine *vn_engine_create(const vn_config *config);

void vn_engine_destroy(vn_engine *engine);

/* Main Loop */

bool vn_engine_running(const vn_engine *engine);

void vn_engine_poll(vn_engine *engine);

void vn_engine_update(vn_engine *engine);

void vn_engine_render(vn_engine *engine);

/* Timing */

float vn_engine_delta_time(const vn_engine *engine);

double vn_engine_time(const vn_engine *engine);

/* Window */

uint32_t vn_engine_width(const vn_engine *engine);

uint32_t vn_engine_height(const vn_engine *engine);

#ifdef __cplusplus
}
#endif

#endif
