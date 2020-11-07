#include"world/target/Target.h"

#include"util/Utils.h"
#include"world/model/mesh/Mesh.h"

#include<stdlib.h>
#include<string.h>

Target target_load(const char *mesh_path, IC_ERROR_CODE *error_code)
{
    Target res = {0};

    Mesh *mesh = malloc(sizeof(Mesh));
    IC_ERROR_CODE ec;
    Mesh loaded_mesh = mesh_load_from_obj(mesh_path, &ec);
    if (ec != IC_NO_ERROR)
    {
        if (error_code) *error_code = ec;
        free(mesh);
        return res;
    }
    memcpy(mesh, &loaded_mesh, sizeof(Mesh));

    res.model.mesh = mesh;
    return res;
}

IC_ERROR_CODE target_create(Target *dest)
{
    return mesh_create((Mesh*)dest->model.mesh);
}

void target_destroy(const Target *target)
{
    mesh_destroy(target->model.mesh);
    free((void*)target->model.mesh);
}