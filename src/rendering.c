#include "rendering.h"
#include "scene.h"
#include "terrain.h"
#include <stddef.h>

static RenderTexture scene_render_target = {0};

static inline void render_scene(void) {
    size_t i = 0;
    Entity *entity = {0};

    while ((entity = scene_get_entity(i++))) {
        if (entity->is_destroyed)
            continue;

        ModelData *model_data = scene_entity_get_model(entity);

        Matrix transform = entity->transform;

        DrawMesh(model_data->model.meshes[0], model_data->model.materials[0],
                 transform);
    }

    // Terrain mesh
    Mesh *terrain_mesh = terrain_get_mesh();
    if (terrain_mesh)
        DrawMesh(terrain.mesh, terrain.material, MatrixTranslate(0, -0.02, 0));
}

void rendering_render(Camera camera) {
    if (!scene_render_target.id || IsWindowResized()) {
        if (scene_render_target.id)
            UnloadRenderTexture(scene_render_target);
        scene_render_target =
            LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    }

    BeginTextureMode(scene_render_target);
    ClearBackground((Color){0});
    BeginMode3D(camera);

    render_scene();

    EndMode3D();
    EndTextureMode();

    // Actual draw loop
    BeginDrawing();
    ClearBackground(BLACK);

    // Skybox
    scene_render_skybox(camera);

    // 3D scene
    DrawTextureRec(scene_render_target.texture,
                   (Rectangle){0, 0, (float)scene_render_target.texture.width,
                               (float)-scene_render_target.texture.height},
                   (Vector2){0, 0}, WHITE);

    EndDrawing();
}
