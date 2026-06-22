#pragma once

#ifndef VN_SPRITE_H
#define VN_SPRITE_H

#include <SDL3/SDL.h>

#include <stdbool.h>

typedef struct vn_sprite {
  SDL_Texture *texture;

  SDL_FRect source;
  bool has_source;

  SDL_FRect rect;

  float alpha;
  int z;
  bool visible;
} vn_sprite;

vn_sprite *vn_sprite_create(SDL_Texture *texture);
void vn_sprite_destroy(vn_sprite *sprite);

void vn_sprite_set_rect(vn_sprite *sprite, float x, float y, float w, float h);
void vn_sprite_set_source(vn_sprite *sprite, float x, float y, float w,
                          float h);
void vn_sprite_clear_source(vn_sprite *sprite);

void vn_sprite_set_alpha(vn_sprite *sprite, float alpha);
void vn_sprite_set_z(vn_sprite *sprite, int z);
void vn_sprite_set_visible(vn_sprite *sprite, bool visible);

/**
 * Makes the sprite fill the target area while preserving aspect ratio.
 * Useful for backgrounds.
 */
bool vn_sprite_cover(vn_sprite *sprite, float width, float height);

void vn_sprite_render(const vn_sprite *sprite, SDL_Renderer *renderer);

#endif
