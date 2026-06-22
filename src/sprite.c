#include <vnovel/sprite.h>

#include <stdlib.h>

vn_sprite *vn_sprite_create(SDL_Texture *texture) {
  if (!texture) {
    return NULL;
  }

  vn_sprite *sprite = calloc(1, sizeof(*sprite));
  if (!sprite) {
    return NULL;
  }

  sprite->texture = texture;
  sprite->rect = (SDL_FRect){0.0f, 0.0f, 0.0f, 0.0f};
  sprite->source = (SDL_FRect){0.0f, 0.0f, 0.0f, 0.0f};
  sprite->has_source = false;
  sprite->alpha = 1.0f;
  sprite->z = 0;
  sprite->visible = true;

  return sprite;
}

void vn_sprite_destroy(vn_sprite *sprite) { free(sprite); }

void vn_sprite_set_rect(vn_sprite *sprite, float x, float y, float w, float h) {
  if (!sprite) {
    return;
  }

  sprite->rect = (SDL_FRect){x, y, w, h};
}

void vn_sprite_set_source(vn_sprite *sprite, float x, float y, float w,
                          float h) {
  if (!sprite) {
    return;
  }

  sprite->source = (SDL_FRect){x, y, w, h};
  sprite->has_source = true;
}

void vn_sprite_clear_source(vn_sprite *sprite) {
  if (!sprite) {
    return;
  }

  sprite->has_source = false;
}

void vn_sprite_set_alpha(vn_sprite *sprite, float alpha) {
  if (!sprite) {
    return;
  }

  if (alpha < 0.0f) {
    alpha = 0.0f;
  }

  if (alpha > 1.0f) {
    alpha = 1.0f;
  }

  sprite->alpha = alpha;
}

void vn_sprite_set_z(vn_sprite *sprite, int z) {
  if (!sprite) {
    return;
  }

  sprite->z = z;
}

void vn_sprite_set_visible(vn_sprite *sprite, bool visible) {
  if (!sprite) {
    return;
  }

  sprite->visible = visible;
}

bool vn_sprite_cover(vn_sprite *sprite, float width, float height) {
  if (!sprite || !sprite->texture || width <= 0.0f || height <= 0.0f) {
    return false;
  }

  float texture_w = 0.0f;
  float texture_h = 0.0f;

  if (!SDL_GetTextureSize(sprite->texture, &texture_w, &texture_h)) {
    return false;
  }

  if (texture_w <= 0.0f || texture_h <= 0.0f) {
    return false;
  }

  float target_ratio = width / height;
  float texture_ratio = texture_w / texture_h;

  SDL_FRect source = {0.0f, 0.0f, texture_w, texture_h};

  if (texture_ratio > target_ratio) {
    float cropped_w = texture_h * target_ratio;
    source.x = (texture_w - cropped_w) * 0.5f;
    source.w = cropped_w;
  } else if (texture_ratio < target_ratio) {
    float cropped_h = texture_w / target_ratio;
    source.y = (texture_h - cropped_h) * 0.5f;
    source.h = cropped_h;
  }

  sprite->source = source;
  sprite->has_source = true;
  sprite->rect = (SDL_FRect){0.0f, 0.0f, width, height};

  return true;
}

void vn_sprite_render(const vn_sprite *sprite, SDL_Renderer *renderer) {
  if (!sprite || !renderer || !sprite->texture || !sprite->visible) {
    return;
  }

  Uint8 alpha = (Uint8)(sprite->alpha * 255.0f);

  SDL_SetTextureBlendMode(sprite->texture, SDL_BLENDMODE_BLEND);
  SDL_SetTextureAlphaMod(sprite->texture, alpha);

  const SDL_FRect *source = sprite->has_source ? &sprite->source : NULL;

  SDL_RenderTexture(renderer, sprite->texture, source, &sprite->rect);
}
