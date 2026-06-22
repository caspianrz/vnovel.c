#include <vnovel/scene.h>

#include <stdlib.h>

vn_scene *vn_scene_create(void) {
  vn_scene *scene = calloc(1, sizeof(*scene));
  if (!scene) {
    return NULL;
  }

  for (int i = 0; i < VN_LAYER_COUNT; i++) {
    vn_layer_init(&scene->layers[i]);
  }

  return scene;
}

void vn_scene_destroy(vn_scene *scene) {
  if (!scene) {
    return;
  }

  for (int i = 0; i < VN_LAYER_COUNT; i++) {
    vn_layer_destroy(&scene->layers[i]);
  }

  free(scene);
}

vn_layer *vn_scene_get_layer(vn_scene *scene, vn_layer_id layer_id) {
  if (!scene || layer_id < 0 || layer_id >= VN_LAYER_COUNT) {
    return NULL;
  }

  return &scene->layers[layer_id];
}

bool vn_scene_add_sprite(vn_scene *scene, vn_layer_id layer_id,
                         vn_sprite *sprite) {
  vn_layer *layer = vn_scene_get_layer(scene, layer_id);

  if (!layer) {
    return false;
  }

  return vn_layer_add_sprite(layer, sprite);
}

void vn_scene_render(const vn_scene *scene, SDL_Renderer *renderer) {
  if (!scene || !renderer) {
    return;
  }

  for (int i = 0; i < VN_LAYER_COUNT; i++) {
    vn_layer_render(&scene->layers[i], renderer);
  }
}
