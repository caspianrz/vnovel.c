#include <vnovel/layer.h>

#include <stdlib.h>
#include <string.h>

void vn_layer_init(vn_layer *layer) {
  if (!layer) {
    return;
  }

  layer->sprites = NULL;
  layer->count = 0;
  layer->capacity = 0;
  layer->visible = true;
}

void vn_layer_destroy(vn_layer *layer) {
  if (!layer) {
    return;
  }

  for (size_t i = 0; i < layer->count; i++) {
    vn_sprite_destroy(layer->sprites[i]);
  }

  free(layer->sprites);

  layer->sprites = NULL;
  layer->count = 0;
  layer->capacity = 0;
  layer->visible = true;
}

bool vn_layer_add_sprite(vn_layer *layer, vn_sprite *sprite) {
  if (!layer || !sprite) {
    return false;
  }

  if (layer->count == layer->capacity) {
    size_t new_capacity = layer->capacity == 0 ? 8 : layer->capacity * 2;

    vn_sprite **new_sprites =
        realloc(layer->sprites, new_capacity * sizeof(*new_sprites));

    if (!new_sprites) {
      return false;
    }

    layer->sprites = new_sprites;
    layer->capacity = new_capacity;
  }

  size_t index = layer->count;

  while (index > 0 && layer->sprites[index - 1]->z > sprite->z) {
    layer->sprites[index] = layer->sprites[index - 1];
    index--;
  }

  layer->sprites[index] = sprite;
  layer->count++;

  return true;
}

void vn_layer_set_visible(vn_layer *layer, bool visible) {
  if (!layer) {
    return;
  }

  layer->visible = visible;
}

void vn_layer_render(const vn_layer *layer, SDL_Renderer *renderer) {
  if (!layer || !renderer || !layer->visible) {
    return;
  }

  for (size_t i = 0; i < layer->count; i++) {
    vn_sprite_render(layer->sprites[i], renderer);
  }
}
