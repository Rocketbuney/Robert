#include "panel.h"
#include <stdlib.h>
#include <string.h>
#include "colors.h"

struct panel_t {
    int contentWidth, contentHeight;
    int titleBarWidth, titleBarHeight;
    int x, y;
    char* title;
};

static const int OUTLINE_THICKNESS = 3;


static void drawPanelTitle(panel_t *p, sfRenderWindow *w) {
    sfFont *font = sfFont_createFromFile("res/arial.ttf");
    sfText *text = sfText_create();
    
    sfText_setString(text, p->title);
    sfText_setFont(text, font);
    sfText_setCharacterSize(text, 24);
    sfText_setColor(text, textColor);    
    sfText_setPosition(text, (sfVector2f){ p->x + 10, p->y + 5 });

    sfRenderWindow_drawText(w, text, NULL);
}

panel_t *createPanel(int x, int y, int contentWidth, int contentHeight, char *title) {
    panel_t *p = (panel_t*)malloc(sizeof(panel_t));
    memset(p, 0, sizeof(*p));

    p->contentWidth = contentWidth;
    p->contentHeight = contentHeight;
    p->titleBarWidth = contentWidth;
    p->titleBarHeight = 40;
    p->x = x;
    p->y = y;
    p->title = title;

    return p;
}

void drawPanel(panel_t *p, sfRenderWindow *w) {
    sfRectangleShape *contentRect = sfRectangleShape_create();
    sfRectangleShape *titleBar = sfRectangleShape_create();
    sfRectangleShape_setSize(contentRect, (sfVector2f){ p->contentWidth, p->contentHeight });
    sfRectangleShape_setPosition(contentRect, (sfVector2f){ p->x, p->y + p->titleBarHeight});

    sfRectangleShape_setFillColor(contentRect, panelBg);
    sfRectangleShape_setOutlineColor(contentRect, panelBoarder);
    sfRectangleShape_setOutlineThickness(contentRect, OUTLINE_THICKNESS);

    sfRectangleShape_setSize(titleBar, (sfVector2f){ p->titleBarWidth + (OUTLINE_THICKNESS * 2), p->titleBarHeight });
    sfRectangleShape_setPosition(titleBar, (sfVector2f){ p->x - OUTLINE_THICKNESS, p->y });

    sfRectangleShape_setFillColor(titleBar, titleBg);

    sfRenderWindow_drawRectangleShape(w, contentRect, NULL);
    sfRenderWindow_drawRectangleShape(w, titleBar, NULL);
    drawPanelTitle(p, w);
}
