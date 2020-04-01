#ifndef PANELS_H
#define PANELS_H
#ifdef CONTROL
#include "microui.h"

void process_frame(mu_Context *ctx);
void render_frame(mu_Context *ctx);
void process_input(mu_Context *ctx);
#endif
#endif
