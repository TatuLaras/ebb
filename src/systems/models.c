#include "models.h"

#include "assets.h"
#include "common.h"
#include "entities.h"
#include "firewatch.h"
#include "model_vector.h"
#include "texture_load.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static ModelVector models = {0};
static const char *asset_directory = 0;

void models_set_asset_directory(const char *dir) {
    asset_directory = dir;
}

Model *models_get(ModelHandle handle) {
    return modelvec_get(&models, handle);
}

static inline void load_model(const char *filepath, ModelHandle handle) {
    // Preserve textures
    Texture texture = {0};
    Texture unlit_texture = {0};
    if (models.data[handle].materialCount) {
        texture =
            models.data[handle].materials[0].maps[MATERIAL_MAP_DIFFUSE].texture;
        unlit_texture = models.data[handle]
                            .materials[0]
                            .maps[MATERIAL_MAP_DIFFUSE + 1]
                            .texture;
    }

    if (models.data[handle].meshes)
        UnloadModel(models.data[handle]);

    models.data[handle] = LoadModel(filepath);
    assert(models.data[handle].meshes);
    // models.data[handle].materials[0].shader =
    // lighting_scene_get_base_shader();

    // Load old textures
    models.data[handle].materials[0].maps[MATERIAL_MAP_DIFFUSE].texture =
        texture;
    models.data[handle].materials[0].maps[MATERIAL_MAP_DIFFUSE + 1].texture =
        unlit_texture;
}

static inline void load_texture(const char *filepath, ModelHandle handle) {
    Model *model = modelvec_get(&models, handle);
    assert(model);
    assert(model->materialCount);
    assert(model->meshCount);
    texture_load_model_texture(filepath, model);
}

void sysinit_models(void) {
    if (models.data)
        return;

    if (!asset_directory) {
        printf("ERROR (models): no asset directory set with "
               "models_set_asset_directory()\n");
        return;
    }

    models = modelvec_init();
    // So we can use model handle 0 as a null value
    modelvec_append(&models, (Model){0});

    EntityHandleVector entities = entities_query(COMPONENT_ID_MODEL);

    for (size_t i = 0; i < entities.data_used; i++) {
        ModelComponent *model = components_get_ModelComponent(i);
        assert(model);

        // Look for already loaded model
        int match_found = 0;
        for (size_t j = 0; j < entities.data_used; j++) {
            ModelComponent *potential_match = components_get_ModelComponent(j);
            assert(potential_match);
            if (potential_match->model_handle &&
                potential_match->asset_handle == model->asset_handle) {
                model->model_handle = potential_match->model_handle;
                match_found = 1;
                break;
            }
        }

        if (match_found)
            continue;

        // If not found, load model
        model->model_handle = modelvec_append(&models, (Model){0});

        char *asset_filename = assets_get_name(model->asset_handle);
        assert(asset_filename);

        char texture_filepath[MAX_PATH_LENGTH] = {0};
        char model_filepath[MAX_PATH_LENGTH] = {0};

        strcpy(texture_filepath, asset_directory);
        strcat(texture_filepath, asset_filename);
        strcpy(model_filepath, asset_directory);
        strcat(model_filepath, asset_filename);

        strcat(model_filepath, ".glb");
        strcat(texture_filepath, ".aseprite");

        firewatch_new_file(model_filepath, model->model_handle, &load_model, 0);
        firewatch_new_file(texture_filepath, model->model_handle, &load_texture,
                           0);
    }
}

void sysdeinit_models(void) {
    //  TODO: Unload models
    for (size_t i = 0; i < models.data_used; i++) {
        if (models.data[i].meshCount) {
            UnloadModel(models.data[i]);
            models.data[i].meshCount = 0;
        }
    }
    modelvec_free(&models);
}
