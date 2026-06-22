#include <vnovel.h>

int main(void) {
  vn_config config = {
      .title = "vnovel.c",
      .width = 1280,
      .height = 720,
  };

  vn_engine *engine = vn_engine_create(&config);

  while (vn_engine_running(engine)) {
    vn_engine_poll(engine);
    vn_engine_update(engine);
    vn_engine_begin_frame(engine);

	vn_engine_end_frame(engine);
  }

  vn_engine_destroy(engine);
  return 0;
}
