#include "rendering.h"
#include "components.h"
#include "entities.h"
#include <assert.h>
#include <raylib.h>
#include <raymath.h>
#include <stddef.h>
#include <stdio.h>

static RenderTexture scene_render_target = {0};

void rendering_render(void) {
    if (!scene_render_target.id || IsWindowResized()) {
        if (scene_render_target.id)
            UnloadRenderTexture(scene_render_target);
        scene_render_target =
            LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    }

    EntityHandle camera_entity = 0;
    if (entities_query_one(COMPONENT_ID_CAMERA, &camera_entity)) {
        fprintf(stderr, "ERROR (rendering_render): No entity with Camera "
                        "component in the scene.");
        return;
    };

    Camera *camera = components_get_Camera(camera_entity);
    assert(camera);

    EntityHandleVector entities =
        entities_query(COMPONENT_ID_MESH | COMPONENT_ID_RENDERABLE);

    BeginTextureMode(scene_render_target);
    ClearBackground((Color){0});
    BeginMode3D(*camera);

    for (size_t i = 0; i < entities.data_used; i++) {
        //  TODO: Does a linear search each time, make method that returns an
        //  array of component pointers.
        Mesh *mesh = components_get_Mesh(i);

        // Optional component
        Matrix *optional_transform = components_get_TransformComponent(i);
        Matrix transform = MatrixIdentity();
        if (optional_transform)
            transform = *optional_transform;

        DrawMesh(*mesh, LoadMaterialDefault(), transform);
    }

    EndMode3D();
    EndTextureMode();

    // Actual draw loop
    BeginDrawing();
    ClearBackground(BLACK);

    // Skybox
    // scene_render_skybox(camera);

    // 3D scene
    DrawTextureRec(scene_render_target.texture,
                   (Rectangle){0, 0, (float)scene_render_target.texture.width,
                               (float)-scene_render_target.texture.height},
                   (Vector2){0, 0}, WHITE);

    EndDrawing();
}
