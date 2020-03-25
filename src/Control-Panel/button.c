#include "button.h"
#include <stdlib.h>
#include <string.h>

button_t *createButton(int x, int y, char *content) {
    button_t *button = (button_t*)malloc(sizeof(button_t));
    memset(button, 0, sizeof(*button));

    button->x = x;
    button->y = y;
    button->content = content;

    return button;
}

void freeButton(button_t *b) {
    free(b);
}

