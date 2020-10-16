#pragma once

#include"Core.h"

typedef struct _Skybox_Data Skybox_Data;

struct _Skybox
{
    const char *left_tex_path;
    const char *right_tex_path;
    const char *bottom_tex_path;
    const char *top_tex_path;
    const char *front_tex_path;
    const char *back_tex_path;

    float size;

    const Skybox_Data *data;
};

typedef struct _Skybox Skybox;

IC_ERROR_CODE skybox_create(Skybox *dest);
void skybox_destroy(const Skybox *skybox);

void skybox_render(const Skybox *skybox);