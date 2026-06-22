#pragma once

#ifndef VN_LAYER_H
#define VN_LAYER_H

#include <vnovel/sprite.h>

#include <SDL3/SDL.h>

#include <stdbool.h>
#include <stddef.h>

typedef struct vn_layer {
  vn_sprite **sprites;
  size_t count;
  size_t capacity;
  bool visible;
} vn_layer;

void vn_layer_init(vn_layer *layer);
void vn_layer_destroy(vn_layer *layer);

bool vn_layer_add_sprite(vn_layer *layer, vn_sprite *sprite);

void vn_layer_set_visible(vn_layer *layer, bool visible);
void vn_layer_render(const vn_layer *layer, SDL_Renderer *renderer);

#endif
