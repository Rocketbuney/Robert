#if !defined(PANEL_H)
#define PANEL_H
#include <SFML/Graphics.h>

typedef struct panel_t panel_t;

panel_t *createPanel(int x, int y, int contentWidth, int contentHeight, char *title);
void drawPanel(panel_t*, sfRenderWindow*);

#endif // PANEL_H
