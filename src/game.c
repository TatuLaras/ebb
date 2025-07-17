#include "game.h"

#include "assets.h"
#include "components.h"
#include "entities.h"
#include "systems.h"
#include "systems/models.h"
#include "systems/rendering.h"
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

const char *skybox_directory = "skyboxes/";
const char *asset_directory = "assets/";
const char *terrain_texture_directory = "terrain_textures/";

static inline void handle_inputs(void) {
    // if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
    //     orbital_camera_update(&camera, 0);
    //     return;
    // }
}

static inline void setup_systems(void) {
    systems_init();
    systems_add_update(&sysupdate_rendering);
    systems_add_init(&sysinit_models);
    systems_add_deinit(&sysdeinit_models);
}

static inline void setup_entities_components(void) {
    entities_init();

    AssetHandle barrel_asset = 0;
    if (assets_get_handle("barrel", &barrel_asset)) {
        printf("Asset not found\n");
        return;
    }

    EntityHandle barrel = entities_new();
    components_add_Renderable(barrel, 0);
    components_add_ModelComponent(
        barrel, (ModelComponent){.asset_handle = barrel_asset});

    EntityHandle barrel2 = entities_new();
    components_add_Renderable(barrel2, 0);
    components_add_ModelComponent(
        barrel2, (ModelComponent){.asset_handle = barrel_asset});
    components_add_TransformComponent(
        barrel2, MatrixMultiply(MatrixRotateZ(2), MatrixTranslate(1, -0.5, 0)));

    EntityHandle camera = entities_new();
    components_add_Camera(camera, (Camera){.position = {0, 2, 5},
                                           .up = {0.0f, 1.0f, 0.0f},
                                           .fovy = 45.0f});
}

int game_init(void) {
    SetTraceLogLevel(LOG_WARNING);
    assets_fetch_all("assets/");
    models_set_asset_directory("assets/");
    setup_systems();
    setup_entities_components();

    systems_run_init();
    return 0;
}

void game_main(void) {
    while (!WindowShouldClose()) {
        handle_inputs();
        systems_run_update();
    }
}

void game_deinit(void) {
    systems_run_deinit();
    entities_free();
    components_free();
    systems_free();
}
