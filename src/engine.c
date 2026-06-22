#include <vnovel.h>

#include <limits.h>

#define VN_MAX_DELTA_TIME 0.25f

struct vn_engine {
  SDL_Window *window;
  SDL_Renderer *renderer;

  bool running;
  bool owns_sdl_video;

  Uint32 width;
  Uint32 height;

  Uint64 last_counter;
  Uint64 performance_frequency;

  double time;
  float delta_time;
};

static void vn__log_sdl_error(const char *message) {
  SDL_Log("%s: %s", message, SDL_GetError());
}

static bool vn__valid_config(const vn_config *config) {
  if (!config) {
    return false;
  }

  if (config->width == 0 || config->height == 0) {
    return false;
  }

  if (config->width > INT_MAX || config->height > INT_MAX) {
    return false;
  }

  return true;
}

static void vn__sync_window_size(vn_engine *engine) {
  if (!engine || !engine->window) {
    return;
  }

  int width = 0;
  int height = 0;

  if (!SDL_GetWindowSize(engine->window, &width, &height)) {
    vn__log_sdl_error("SDL_GetWindowSize failed");
    return;
  }

  if (width > 0 && height > 0) {
    engine->width = width;
    engine->height = height;
  }
}

static void vn__engine_cleanup(vn_engine *engine) {
  if (!engine) {
    return;
  }

  if (engine->renderer) {
    SDL_DestroyRenderer(engine->renderer);
  }

  if (engine->window) {
    SDL_DestroyWindow(engine->window);
  }

  if (engine->owns_sdl_video) {
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
  }

  free(engine);
}

vn_engine *vn_engine_create(const vn_config *config) {
  if (!vn__valid_config(config)) {
    return NULL;
  }

  bool video_was_initialized =
      (SDL_WasInit(SDL_INIT_VIDEO) & SDL_INIT_VIDEO) != 0;

  if (!video_was_initialized && !SDL_Init(SDL_INIT_VIDEO)) {
    vn__log_sdl_error("SDL_Init failed");
    return NULL;
  }

  vn_engine *engine = calloc(1, sizeof(*engine));

  if (!engine) {
    SDL_Log("vn_engine allocation failed");

    if (!video_was_initialized) {
      SDL_QuitSubSystem(SDL_INIT_VIDEO);
    }

    return NULL;
  }

  engine->owns_sdl_video = !video_was_initialized;
  engine->running = true;

  engine->width = config->width;
  engine->height = config->height;

  const char *title = config->title ? config->title : "vnovel.c";

  engine->window =
      SDL_CreateWindow(title, (int)config->width, (int)config->height, 0);

  if (!engine->window) {
    vn__log_sdl_error("SDL_CreateWindow failed");
    vn__engine_cleanup(engine);
    return NULL;
  }

  engine->renderer = SDL_CreateRenderer(engine->window, NULL);

  if (!engine->renderer) {
    vn__log_sdl_error("SDL_CreateRenderer failed");
    vn__engine_cleanup(engine);
    return NULL;
  }

  engine->performance_frequency = SDL_GetPerformanceFrequency();
  engine->last_counter = SDL_GetPerformanceCounter();

  vn__sync_window_size(engine);

  return engine;
}

void vn_engine_destroy(vn_engine *engine) { vn__engine_cleanup(engine); }

bool vn_engine_running(const vn_engine *engine) {
  return engine && engine->running;
}

void vn_engine_stop(vn_engine *engine) {
  if (engine) {
    engine->running = false;
  }
}

void vn_engine_poll(vn_engine *engine) {
  if (!engine) {
    return;
  }

  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_EVENT_QUIT:
      engine->running = false;
      break;

    case SDL_EVENT_WINDOW_RESIZED:
    case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
      vn__sync_window_size(engine);
      break;

    default:
      break;
    }
  }
}

void vn_engine_update(vn_engine *engine) {
  if (!engine) {
    return;
  }

  Uint64 current_counter = SDL_GetPerformanceCounter();

  float delta_time = (float)((double)(current_counter - engine->last_counter) /
                             (double)engine->performance_frequency);

  if (delta_time > VN_MAX_DELTA_TIME) {
    delta_time = VN_MAX_DELTA_TIME;
  }

  engine->delta_time = delta_time;
  engine->time += delta_time;
  engine->last_counter = current_counter;
}

void vn_engine_begin_frame(vn_engine *engine) {
  if (!engine || !engine->renderer) {
    return;
  }

  SDL_SetRenderDrawColor(engine->renderer, 0, 0, 0, 255);
  SDL_RenderClear(engine->renderer);
}

void vn_engine_end_frame(vn_engine *engine) {
  if (!engine || !engine->renderer) {
    return;
  }

  SDL_RenderPresent(engine->renderer);
}

void vn_engine_render(vn_engine *engine) {
  vn_engine_begin_frame(engine);
  vn_engine_end_frame(engine);
}

float vn_engine_delta_time(const vn_engine *engine) {
  return engine ? engine->delta_time : 0.0f;
}

double vn_engine_time(const vn_engine *engine) {
  return engine ? engine->time : 0.0;
}

Uint32 vn_engine_width(const vn_engine *engine) {
  return engine ? engine->width : 0;
}

Uint32 vn_engine_height(const vn_engine *engine) {
  return engine ? engine->height : 0;
}

SDL_Window *vn_engine_window(vn_engine *engine) {
  return engine ? engine->window : NULL;
}

SDL_Renderer *vn_engine_renderer(vn_engine *engine) {
  return engine ? engine->renderer : NULL;
}
