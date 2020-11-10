#pragma once

#include"world/model/Model.h"
#include"util/math/Vec.h"

#include"Core.h"

struct _Target
{
    Model model;

    IC_BOOL active;
};

typedef struct _Target Target;

Target target_load(const char *mesh_path, IC_ERROR_CODE *error_code);
IC_ERROR_CODE target_create(Target *dest);
void target_destroy(const Target *target);