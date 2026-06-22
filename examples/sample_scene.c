#include <vnovel.h>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <stdio.h>

static SDL_Texture *load_texture_png(vn_engine *engine, const char *path) {
  if (!engine || !path) {
    return NULL;
  }

  SDL_Renderer *renderer = vn_engine_renderer(engine);
  if (!renderer) {
    SDL_Log("Cannot load PNG '%s': renderer is NULL", path);
    return NULL;
  }

  SDL_Texture *texture = IMG_LoadTexture(renderer, path);
  if (!texture) {
    SDL_Log("Failed to load PNG '%s': %s", path, SDL_GetError());
    return NULL;
  }

  /*
   * Important for PNG transparency.
   */
  SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

  return texture;
}

static SDL_Texture *load_texture_bmp(vn_engine *engine, const char *path) {
  SDL_Renderer *renderer = vn_engine_renderer(engine);

  SDL_Surface *surface = SDL_LoadBMP(path);
  if (!surface) {
    SDL_Log("Failed to load BMP '%s': %s", path, SDL_GetError());
    return NULL;
  }

  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_DestroySurface(surface);

  if (!texture) {
    SDL_Log("Failed to create texture from '%s': %s", path, SDL_GetError());
    return NULL;
  }

  return texture;
}

int main(void) {
  vn_config config = {
      .title = "Visual Novel Scene Example",
      .width = 1280,
      .height = 720,
  };

  vn_engine *engine = vn_engine_create(&config);
  if (!engine) {
    SDL_Log("Failed to create engine");
    return 1;
  }

  /*
   * Background uses your vn_background API.
   *
   * Example path:
   *   examples/assets/background.bmp
   */
  vn_background *background =
      vn_background_load(engine, "assets/backgrounds/sunset.png");

  if (!background) {
    SDL_Log("Failed to load background");
    vn_engine_destroy(engine);
    return 1;
  }

  vn_background_set_fit(background, VN_BACKGROUND_FIT_COVER);

  /*
   * Character sprite.
   *
   * Since vn_sprite_create() expects SDL_Texture *,
   * this example loads the texture directly with SDL.
   *
   * For now this uses BMP so you do not need SDL_image yet.
   */
  SDL_Texture *character_texture =
      load_texture_png(engine, "assets/characters/sakura.png");

  if (!character_texture) {
    vn_background_destroy(background);
    vn_engine_destroy(engine);
    return 1;
  }

  vn_sprite *character = vn_sprite_create(character_texture);
  if (!character) {
    SDL_DestroyTexture(character_texture);
    vn_background_destroy(background);
    vn_engine_destroy(engine);
    return 1;
  }

  /*
   * Position the character.
   *
   * Example:
   *   x = 760
   *   y = 120
   *   w = 360
   *   h = 600
   */
  vn_sprite_set_rect(character, 760.0f, 120.0f, 360.0f, 600.0f);

  vn_scene *scene = vn_scene_create();
  if (!scene) {
    vn_sprite_destroy(character);
    SDL_DestroyTexture(character_texture);
    vn_background_destroy(background);
    vn_engine_destroy(engine);
    return 1;
  }

  vn_scene_add_sprite(scene, VN_LAYER_CHARACTER, character);

  while (vn_engine_running(engine)) {
    vn_engine_poll(engine);
    vn_engine_update(engine);

    vn_engine_begin_frame(engine);

    vn_background_render(background, engine);
    vn_engine_render_scene(engine, scene);

    vn_engine_end_frame(engine);
  }

  vn_scene_destroy(scene);

  SDL_DestroyTexture(character_texture);

  vn_background_destroy(background);
  vn_engine_destroy(engine);

  return 0;
}
