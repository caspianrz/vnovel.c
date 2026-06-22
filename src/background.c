#include <vnovel.h>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <stdlib.h>

struct vn_background {
  SDL_Texture *texture;

  float width;
  float height;

  vn_background_fit fit;
  float alpha;
};

static float vn__min_float(float a, float b) { return a < b ? a : b; }

static float vn__max_float(float a, float b) { return a > b ? a : b; }

static float vn__clamp_float(float value, float min, float max) {
  if (value < min) {
    return min;
  }

  if (value > max) {
    return max;
  }

  return value;
}

static SDL_FRect
vn__background_destination_rect(const vn_background *background,
                                float target_width, float target_height) {
  SDL_FRect dst = {
      .x = 0.0f,
      .y = 0.0f,
      .w = target_width,
      .h = target_height,
  };

  if (!background || background->width <= 0.0f || background->height <= 0.0f) {
    return dst;
  }

  switch (background->fit) {
  case VN_BACKGROUND_FIT_STRETCH:
    dst.x = 0.0f;
    dst.y = 0.0f;
    dst.w = target_width;
    dst.h = target_height;
    break;

  case VN_BACKGROUND_FIT_CONTAIN: {
    float scale_x = target_width / background->width;
    float scale_y = target_height / background->height;
    float scale = vn__min_float(scale_x, scale_y);

    dst.w = background->width * scale;
    dst.h = background->height * scale;
    dst.x = (target_width - dst.w) * 0.5f;
    dst.y = (target_height - dst.h) * 0.5f;
  } break;

  case VN_BACKGROUND_FIT_COVER: {
    float scale_x = target_width / background->width;
    float scale_y = target_height / background->height;
    float scale = vn__max_float(scale_x, scale_y);

    dst.w = background->width * scale;
    dst.h = background->height * scale;
    dst.x = (target_width - dst.w) * 0.5f;
    dst.y = (target_height - dst.h) * 0.5f;
  } break;

  case VN_BACKGROUND_FIT_CENTER:
    dst.w = background->width;
    dst.h = background->height;
    dst.x = (target_width - dst.w) * 0.5f;
    dst.y = (target_height - dst.h) * 0.5f;
    break;

  default:
    break;
  }

  return dst;
}

vn_background *vn_background_load(vn_engine *engine, const char *path) {
  if (!engine || !path) {
    return NULL;
  }

  SDL_Renderer *renderer = vn_engine_renderer(engine);

  if (!renderer) {
    return NULL;
  }

  vn_background *background = calloc(1, sizeof(*background));

  if (!background) {
    SDL_Log("vn_background allocation failed");
    return NULL;
  }

  background->texture = IMG_LoadTexture(renderer, path);

  if (!background->texture) {
    SDL_Log("IMG_LoadTexture failed for '%s': %s", path, SDL_GetError());
    free(background);
    return NULL;
  }

  if (!SDL_GetTextureSize(background->texture, &background->width,
                          &background->height)) {
    SDL_Log("SDL_GetTextureSize failed for '%s': %s", path, SDL_GetError());
    SDL_DestroyTexture(background->texture);
    free(background);
    return NULL;
  }

  SDL_SetTextureBlendMode(background->texture, SDL_BLENDMODE_BLEND);

  background->fit = VN_BACKGROUND_FIT_COVER;
  background->alpha = 1.0f;

  return background;
}

void vn_background_destroy(vn_background *background) {
  if (!background) {
    return;
  }

  if (background->texture) {
    SDL_DestroyTexture(background->texture);
  }

  free(background);
}

void vn_background_render(vn_background *background, vn_engine *engine) {
  if (!background || !engine || !background->texture) {
    return;
  }

  SDL_Renderer *renderer = vn_engine_renderer(engine);

  if (!renderer) {
    return;
  }

  uint32_t engine_width = vn_engine_width(engine);
  uint32_t engine_height = vn_engine_height(engine);

  if (engine_width == 0 || engine_height == 0) {
    return;
  }

  float target_width = (float)engine_width;
  float target_height = (float)engine_height;

  SDL_FRect dst =
      vn__background_destination_rect(background, target_width, target_height);

  Uint8 alpha =
      (Uint8)(vn__clamp_float(background->alpha, 0.0f, 1.0f) * 255.0f);

  SDL_SetTextureAlphaMod(background->texture, alpha);

  SDL_RenderTexture(renderer, background->texture, NULL, &dst);
}

void vn_background_set_fit(vn_background *background, vn_background_fit fit) {
  if (!background) {
    return;
  }

  background->fit = fit;
}

void vn_background_set_alpha(vn_background *background, float alpha) {
  if (!background) {
    return;
  }

  background->alpha = vn__clamp_float(alpha, 0.0f, 1.0f);
}

float vn_background_width(const vn_background *background) {
  return background ? background->width : 0.0f;
}

float vn_background_height(const vn_background *background) {
  return background ? background->height : 0.0f;
}
