#include "raylib.h"
#include <stddef.h>
#include <stdio.h>

int main(int argc, char **argv) {
    char *model_filepath = 0;
    for (int i = 1; i < argc; i++) {
        //  TODO: Command line args

        model_filepath = argv[i];
        break;
    }

    if (!model_filepath) {
        fprintf(stderr, "Error: No model file was supplied as an argument.\n");
        return 1;
    }

    InitWindow(800, 450, "Bricklayer");

    Model model = LoadModel(model_filepath);

    while (!WindowShouldClose()) {
        // TODO: Update your variables here

        BeginDrawing();

        Color col = {.r = 255};
        ClearBackground(col);

        DrawText("Congrats! You created your first window!", 190, 200, 20,
                 LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
