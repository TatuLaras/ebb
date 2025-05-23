#include "game.h"
#include "raylib.h"
#include <stddef.h>

int main(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        break;
    }

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 450, "Ebb");
    SetTargetFPS(60);

    if (game_init())
        return 1;
    game_main();

    game_deinit();

    CloseWindow();
    return 0;
}
