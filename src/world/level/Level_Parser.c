#include"world/level/Level_Parser.h"
#include"world/level/Level_Mmt.h"
#include"world/rect/Rect.h"

#include"math/Vec.h"
#include"Utils.h"

#include<stdlib.h>
#include<stdint.h>
#include<string.h>

struct _Dyn_Arr
{
    void *arr;
    const uint32_t elem_size;
    uint32_t size, index;
};

typedef struct _Dyn_Arr Dyn_Arr;

void dyn_arr_create(Dyn_Arr *dest)
{
    dest->arr = calloc(10, dest->elem_size);
    dest->size = 10;
    dest->index = 0;
}

void dyn_arr_add(Dyn_Arr *arr, void *elem)
{
    if (arr->index == arr->size)
    {
        arr->arr = realloc(arr->arr, (arr->size *= 2) * arr->elem_size);
    }

    memcpy(arr->arr + (arr->index++ * arr->elem_size), elem, arr->elem_size);
}

void dyn_arr_free(Dyn_Arr *arr)
{
    free(arr->arr);
}

IC_ERROR_CODE level_create_from_file(const char *path, Level *dest)
{
    IC_ERROR_CODE ec;
    const char *source = read_source(path, &ec);
    if (ec != IC_NO_ERROR)
    {
        free((void*)source);
        return ec;
    }

    Dyn_Arr rects = { .elem_size=sizeof(Rect) };
    dyn_arr_create(&rects);

    Vec3 start_position;
    float start_pitch, start_yaw;

    uint32_t num_lines;
    const char **lines = string_split(source, '\n', &num_lines);

    for (uint32_t l = 0; l < num_lines; ++l)
    {
        const char *line = lines[l];
        const char *trimmed_line = string_trim(line);

        uint32_t num_parts;
        const char **parts = string_split(trimmed_line, ' ', &num_parts);

        const char *identifier = parts[0];

        if (strcmp("rect", identifier) == 0)
        {
            Vec3 center = { 0.0f, 0.0f, 0.0f };
            Vec3 extent = { 0.0f, 0.0f, 0.0f };

            for (uint32_t part = 1; part < num_parts; ++part)
            {
                const char *arg_pair = parts[part];

                uint32_t num_arg_elems;
                const char **arg_elems = string_split(arg_pair, '=', &num_arg_elems);

                const char *param = arg_elems[0];
                const char *value = arg_elems[1];

                if (strcmp("center_x", param) == 0)
                {
                    center.x = strtof(value, NULL);
                }
                else if (strcmp("center_y", param) == 0)
                {
                    center.y = strtof(value, NULL);
                }
                else if (strcmp("center_z", param) == 0)
                {
                    center.z = strtof(value, NULL);
                }
                else if (strcmp("h_width", param) == 0)
                {
                    extent.x = strtof(value, NULL);
                }
                else if (strcmp("h_height", param) == 0)
                {
                    extent.y = strtof(value, NULL);
                }
                else if (strcmp("h_depth", param) == 0)
                {
                    extent.z = strtof(value, NULL);
                }

                free((void*)param);
                free((void*)value);

                free((void*)arg_elems);
                free((void*)arg_pair);
            }

            Rect rect = { center, extent };
            dyn_arr_add(&rects, &rect);
        }
        else if (strcmp("player", identifier) == 0)
        {
            for (uint32_t part = 1; part < num_parts; ++part)
            {
                const char *arg_pair = parts[part];

                uint32_t num_arg_elems;
                const char **arg_elems = string_split(arg_pair, '=', &num_arg_elems);

                const char *param = arg_elems[0];
                const char *value = arg_elems[1];

                if (strcmp("start_x", param) == 0)
                {
                    start_position.x = strtof(value, NULL);
                }
                else if (strcmp("start_y", param) == 0)
                {
                    start_position.y = strtof(value, NULL);
                }
                else if (strcmp("start_z", param) == 0)
                {
                    start_position.z = strtof(value, NULL);
                }
                else if (strcmp("start_pitch", param) == 0)
                {
                    start_pitch = strtof(value, NULL);
                }
                else if (strcmp("start_yaw", param) == 0)
                {
                    start_yaw = strtof(value, NULL);
                }

                free((void*)param);
                free((void*)value);

                free((void*)arg_elems);
                free((void*)arg_pair);
            }
        }

        free((void*)identifier);

        free((void*)parts);
        free((void*)trimmed_line);
        free((void*)line);
    }

    free((void*)lines);
    free((void*)source);

    dest->start_position = start_position;
    dest->start_pitch = start_pitch;
    dest->start_yaw = start_yaw;

    Level_Data *data = malloc(sizeof(Level_Data));
    dest->data = data;

    data->num_rects = rects.index;
    data->rects = (Rect*)rects.arr;
    data->rect_index = 0;

    return IC_NO_ERROR;
}