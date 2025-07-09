#include "game.h"

#include "assets.h"
#include "lighting.h"
#include "orbital_controls.h"
#include "rendering.h"
#include "scene.h"
#include "scene_file.h"
#include "skyboxes.h"
#include "systems.h"
#include "terrain.h"
#include "terrain_textures.h"
#include <raylib.h>
#include <stdio.h>

static Camera camera = {0};
const char *skybox_directory = "skyboxes/";
const char *asset_directory = "assets/";
const char *terrain_texture_directory = "terrain_textures/";

static inline void load_scene(const char *scene_filepath) {
    FILE *fp = fopen(scene_filepath, "r");
    if (!fp) {
        perror("WARNING: Could not open scene file");
        return;
    }

    if (scene_file_load(fp, skybox_directory, asset_directory))
        printf("WARNING: Did not load scene from file, either it doesn't exist "
               "or is in the wrong format.\n");

    fclose(fp);
}

static inline void handle_inputs(void) {
    if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
        orbital_camera_update(&camera, 0);
        return;
    }
}

int game_init(void) {
    systems_init();
    // Add systems

    scene_init();
    lighting_scene_init(BLACK, 0, 0);

    assets_fetch_all("assets");
    skyboxes_fetch_all("skyboxes");
    terrain_textures_fetch_all("terrain_textures");

    lighting_scene_set_enabled(1);

    camera.position = (Vector3){0.0f, 6.0f, 6.0f};
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;

    terrain_init(60);
    load_scene("scenes/test.noble");
    scene_load_skybox(skybox_directory);
    terrain_generate_mesh();
    terrain_textures_load_all_selected(terrain_texture_directory);
    return 0;
}

void game_main(void) {
    while (!WindowShouldClose()) {
        handle_inputs();
        systems_run_update();
        rendering_render(camera);
    }
}

void game_deinit(void) {
    scene_free();
    terrain_free();
}
