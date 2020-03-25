#include "panel.h"
#include <stdlib.h>
#include <string.h>
#include "colors.h"

static const int OUTLINE_THICKNESS = 3;

static void drawPanelTitle(panel_t *p, sfRenderWindow *w) {
    sfFont *font = sfFont_createFromFile("res/arial.ttf");
    sfText *text = sfText_create();
    
    sfText_setString(text, p->title);
    sfText_setFont(text, font);
    sfText_setCharacterSize(text, 24);
    sfText_setColor(text, textColor);
    sfText_setPosition(text, (sfVector2f){ p->x + (p->titleBarWidth / 2) - (sfText_getLocalBounds(text).width / 2), p->y + 5 });

    sfRenderWindow_drawText(w, text, NULL);
}

static void drawPanelButtons(panel_t *p, sfRenderWindow *w) {
    for (int i = 0; i < p->numButtons; i++) {
        if(p->buttons[i] != NULL) {
            button_t *b = p->buttons[i];
            sfRectangleShape *buttonRect = sfRectangleShape_create();

            sfRectangleShape_setSize(buttonRect, (sfVector2f){ 48, 16 });
            sfRectangleShape_setPosition(buttonRect, (sfVector2f){ p->x + b->x, p->y + b->y });

            sfRectangleShape_setFillColor(buttonRect, sfWhite);
            sfRectangleShape_setOutlineColor(buttonRect, panelBoarder);
            sfRectangleShape_setOutlineThickness(buttonRect, OUTLINE_THICKNESS);

            sfRenderWindow_drawRectangleShape(w, buttonRect, NULL);
        }
    }
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

    p->numButtons = 0;

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
    drawPanelButtons(p, w);
    sfRenderWindow_drawRectangleShape(w, titleBar, NULL);
    drawPanelTitle(p, w);
}

void addButton(panel_t *p, button_t *b) {
    if (p->numButtons < 48) {
        p->numButtons++;
        p->buttons[p->numButtons - 1] = b;
    }
}

void deletePanel(panel_t *p) {
    free(p);
}

