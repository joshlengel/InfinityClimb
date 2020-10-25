#pragma once

#include"Core.h"

#include<stdint.h>

typedef struct _Mesh_Data Mesh_Data;

struct _Mesh
{
    const float *vertices;
    const float *texture_coords;
    const float *normals;
    uint32_t num_vertices;

    const uint32_t *indices;
    uint32_t num_indices;

    const Mesh_Data *data;
};

typedef struct _Mesh Mesh;

IC_ERROR_CODE mesh_create(Mesh *dest);
void mesh_destroy(const Mesh *mesh);

void mesh_render(const Mesh *mesh);

Mesh mesh_load_from_obj(const char *path, IC_ERROR_CODE *error_code);