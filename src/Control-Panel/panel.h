#if !defined(PANEL_H)
#define PANEL_H
#include <SFML/Graphics.h>
#include "button.h"

typedef struct panel_t {
    int contentWidth, contentHeight;
    int titleBarWidth, titleBarHeight;
    int x, y;
    char* title;

    button_t *buttons[48];
    int numButtons;
} panel_t;

panel_t *createPanel(int x, int y, int contentWidth, int contentHeight, char *title);
void drawPanel(panel_t*, sfRenderWindow*);
void addButton(panel_t*, button_t*);
void deletePanel(panel_t*);

#endif // PANEL_H
