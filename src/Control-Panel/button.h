#if !defined(BUTTON_H)
#define BUTTON_H

typedef struct button_t {
    int x, y;
    int state;
    char *content;
} button_t;

button_t *createButton(int x, int y, char *content);
void freeButton(button_t*);

#endif // BUTTON_H
