#ifdef CONTROL
#include "panels.h"
#include <SDL2/SDL.h>
#include "renderer.h"

static float bg[3] = { 82, 95, 100 };

static const char button_map[256] = {
  [ SDL_BUTTON_LEFT   & 0xff ] =  MU_MOUSE_LEFT,
  [ SDL_BUTTON_RIGHT  & 0xff ] =  MU_MOUSE_RIGHT,
  [ SDL_BUTTON_MIDDLE & 0xff ] =  MU_MOUSE_MIDDLE,
};

static const char key_map[256] = {
  [ SDLK_LSHIFT       & 0xff ] = MU_KEY_SHIFT,
  [ SDLK_RSHIFT       & 0xff ] = MU_KEY_SHIFT,
  [ SDLK_LCTRL        & 0xff ] = MU_KEY_CTRL,
  [ SDLK_RCTRL        & 0xff ] = MU_KEY_CTRL,
  [ SDLK_LALT         & 0xff ] = MU_KEY_ALT,
  [ SDLK_RALT         & 0xff ] = MU_KEY_ALT,
  [ SDLK_RETURN       & 0xff ] = MU_KEY_RETURN,
  [ SDLK_BACKSPACE    & 0xff ] = MU_KEY_BACKSPACE,
};

static int uint8_slider(mu_Context *ctx, unsigned char *value, int low, int high) {
  static float tmp;
  mu_push_id(ctx, &value, sizeof(value));
  tmp = *value;
  int res = mu_slider_ex(ctx, &tmp, low, high, 0, "%.0f", MU_OPT_ALIGNCENTER);
  *value = tmp;
  mu_pop_id(ctx);
  return res;
}

void process_input(mu_Context *ctx) {
  /* handle SDL events */
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    switch (e.type) {
      case SDL_QUIT: exit(EXIT_SUCCESS); break;
      case SDL_MOUSEMOTION: mu_input_mousemove(ctx, e.motion.x, e.motion.y); break;
      case SDL_MOUSEWHEEL: mu_input_scroll(ctx, 0, e.wheel.y * -30); break;
      case SDL_TEXTINPUT: mu_input_text(ctx, e.text.text); break;

      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP: {
        int b = button_map[e.button.button & 0xff];
        if (b && e.type == SDL_MOUSEBUTTONDOWN) { mu_input_mousedown(ctx, e.button.x, e.button.y, b); }
        if (b && e.type ==   SDL_MOUSEBUTTONUP) { mu_input_mouseup(ctx, e.button.x, e.button.y, b);   }
        break;
      }

      case SDL_KEYDOWN:
      case SDL_KEYUP: {
        int c = key_map[e.key.keysym.sym & 0xff];
        if (c && e.type == SDL_KEYDOWN) { mu_input_keydown(ctx, c); }
        if (c && e.type ==   SDL_KEYUP) { mu_input_keyup(ctx, c);   }
        break;
      }
    }
  }
}

void render_frame(mu_Context *ctx) {
  r_clear(mu_color(bg[0], bg[1], bg[2], 255));
  mu_Command *cmd = NULL;
  while (mu_next_command(ctx, &cmd)) {
    switch (cmd->type) {
      case MU_COMMAND_TEXT: r_draw_text(cmd->text.str, cmd->text.pos, cmd->text.color); break;
      case MU_COMMAND_RECT: r_draw_rect(cmd->rect.rect, cmd->rect.color); break;
      case MU_COMMAND_ICON: r_draw_icon(cmd->icon.id, cmd->icon.rect, cmd->icon.color); break;
      case MU_COMMAND_CLIP: r_set_clip_rect(cmd->clip.rect); break;
    }
  }
  r_present();
}

static void info_window(mu_Context *ctx) {
  /* do window */
  if (mu_begin_window_ex(ctx, "Info Window", mu_rect(15, 15, 250, 325), MU_OPT_NORESIZE | MU_OPT_NOCLOSE)) {
    mu_Container *win = mu_get_current_container(ctx);
    win->rect.w = mu_max(win->rect.w, 250);
    win->rect.h = mu_max(win->rect.h, 325);

    mu_end_window(ctx);
  }
}

static void control_window(mu_Context *ctx) {
  /* do window */
  if (mu_begin_window_ex(ctx, "Control Window", mu_rect(280, 15, 305, 325), MU_OPT_NORESIZE | MU_OPT_NOCLOSE)) {
    mu_Container *win = mu_get_current_container(ctx);
    win->rect.w = mu_max(win->rect.w, 305);
    win->rect.h = mu_max(win->rect.h, 325);

    mu_end_window(ctx);
  }
}

static void log_window(mu_Context *ctx) {
  /* do window */
  if (mu_begin_window_ex(ctx, "Log Window", mu_rect(15, 355, 570, 150), MU_OPT_NORESIZE | MU_OPT_NOCLOSE)) {
    mu_Container *win = mu_get_current_container(ctx);
    win->rect.w = mu_max(win->rect.w, 570);
    win->rect.h = mu_max(win->rect.h, 150);

    mu_end_window(ctx);
  }
}

void process_frame(mu_Context *ctx) {
  mu_begin(ctx);
  info_window(ctx);
  control_window(ctx);
  log_window(ctx);
  mu_end(ctx);
}
#endif
