#include <vnovel.h>

int main(void) {
  vn_config config = {
      .title = "vnovel",
      .width = 1280,
      .height = 720,
  };

  vn_engine *engine = vn_engine_create(&config);

  if (!engine) {
    return 1;
  }

  vn_background *bg = vn_background_load(engine, "assets/backgrounds/sunset.png");

  if (!bg) {
    vn_engine_destroy(engine);
    return 1;
  }

  vn_background_set_fit(bg, VN_BACKGROUND_FIT_COVER);

  while (vn_engine_running(engine)) {
    vn_engine_poll(engine);
    vn_engine_update(engine);

    vn_engine_begin_frame(engine);

    vn_background_render(bg, engine);

    vn_engine_end_frame(engine);
  }

  vn_background_destroy(bg);
  vn_engine_destroy(engine);

  return 0;
}
