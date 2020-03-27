#include <SDL2/SDL.h>
#include <stdio.h>
#include "renderer.h"
#include "microui.h"
#include "panels.h"

#define unused(x) ((void) (x))

static int text_width(mu_Font font, const char *text, int len) {
  unused(font);
  if (len == -1) { len = strlen(text); }
  return r_get_text_width(text, len);
}

static int text_height(mu_Font font) {
  unused(font);
  return r_get_text_height();
}

int main(int argc, char **argv) {
  unused(argc);
  unused(argv);

  /* init SDL and renderer */
  SDL_Init(SDL_INIT_EVERYTHING);
  r_init();

  /* init microui */
  mu_Context *ctx = malloc(sizeof(mu_Context));
  mu_init(ctx);
  ctx->text_width = text_width;
  ctx->text_height = text_height;

  /* main loop */
  for (;;) {
    /* process window input */
    process_input(ctx);
    /* process frame */
    process_frame(ctx);
    /* render */
    render_frame(ctx);
  }

  return 0;
}
