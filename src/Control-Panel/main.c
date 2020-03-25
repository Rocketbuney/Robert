#include <stdlib.h>
#include <SFML/Graphics.h>
#include "colors.h"
#include "panel.h"

int main() {
    sfVideoMode mode = { 1600, 1200, 32 };
    sfRenderWindow* window;
    sfEvent event;
    panel_t *infoPanel = createPanel(32, 32, 400, 800, "Info");
    panel_t *logPanel = createPanel(32, 900, 1536, 225, "Log");
    panel_t *controlPanel = createPanel(464, 32, 1100, 800, "Control");

    window = sfRenderWindow_create(mode, "Control Panel", sfClose, NULL);
    if (!window)
        return 1;

    while (sfRenderWindow_isOpen(window)) {
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        }

        sfRenderWindow_clear(window, windowBg);
        drawPanel(infoPanel, window);
        drawPanel(logPanel, window);
        drawPanel(controlPanel, window);
        sfRenderWindow_display(window);
    }

    sfRenderWindow_destroy(window);
    return 0;
}
