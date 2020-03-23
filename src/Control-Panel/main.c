#include <stdlib.h>
#include <SFML/Graphics.h>
#include "colors.h"
#include "panel.h"

int main() {
    sfVideoMode mode = {1600, 1200, 32};
    sfRenderWindow* window;
    sfEvent event;
    panel_t *panel = createPanel(128, 128, 600, 400, "Test Panel");

    window = sfRenderWindow_create(mode, "Control Panel", sfClose, NULL);
    if (!window)
        return 1;

    while (sfRenderWindow_isOpen(window)) {
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        }

        sfRenderWindow_clear(window, windowBg);
        drawPanel(panel, window);
        sfRenderWindow_display(window);
    }

    sfRenderWindow_destroy(window);
    return 0;
}
