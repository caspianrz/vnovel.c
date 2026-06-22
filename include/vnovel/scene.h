#pragma once

#ifndef VN_SCENE_H
#define VN_SCENE_H

#include <vnovel/layer.h>

#include <SDL3/SDL.h>

#include <stdbool.h>

typedef enum vn_layer_id {
  VN_LAYER_BACKGROUND = 0,
  VN_LAYER_BACK,
  VN_LAYER_CHARACTER,
  VN_LAYER_FOREGROUND,
  VN_LAYER_EFFECT,

  /*
   * Reserved for later.
   * You do not need to use this yet.
   */
  VN_LAYER_UI,

  VN_LAYER_COUNT
} vn_layer_id;

typedef struct vn_scene {
  vn_layer layers[VN_LAYER_COUNT];
} vn_scene;

vn_scene *vn_scene_create(void);
void vn_scene_destroy(vn_scene *scene);

vn_layer *vn_scene_get_layer(vn_scene *scene, vn_layer_id layer_id);

bool vn_scene_add_sprite(vn_scene *scene, vn_layer_id layer_id,
                         vn_sprite *sprite);

void vn_scene_render(const vn_scene *scene, SDL_Renderer *renderer);

#endif
