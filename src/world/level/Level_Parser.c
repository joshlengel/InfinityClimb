#include"world/level/Level_Parser.h"
#include"world/level/Level_Mmt.h"
#include"world/rect/Rect.h"

#include"util/math/Vec.h"
#include"util/Utils.h"
#include"util/Vector.h"
#include"util/String.h"

#include<stdlib.h>
#include<stdint.h>
#include<string.h>

IC_ERROR_CODE level_create_from_file(const char *path, Level *dest)
{
    IC_ERROR_CODE ec;
    String source_str = read_source(path, &ec);
    if (ec != IC_NO_ERROR)
    {
        return ec;
    }

    String_View source;
    string_view_create_s(&source, &source_str, 0, UINT32_MAX);

    Vector rects;
    rects.elem_size = sizeof(Rect);
    vector_create(&rects, 10);

    Vec3 start_position;
    float start_pitch, start_yaw;

    uint32_t num_lines;
    String_View *lines = string_view_split(&source, '\n', &num_lines);

    for (uint32_t l = 0; l < num_lines; ++l)
    {
        String_View line = lines[l];
        String_View trimmed_line = string_view_trim(&line);

        uint32_t num_parts;
        String_View *parts = string_view_split(&trimmed_line, ' ', &num_parts);

        String_View identifier = parts[0];

        if (string_view_equals_c_str(&identifier, "rect"))
        {
            Vec3 center = { 0.0f, 0.0f, 0.0f };
            Vec3 extent = { 0.0f, 0.0f, 0.0f };

            for (uint32_t part = 1; part < num_parts; ++part)
            {
                String_View arg_pair = parts[part];

                uint32_t num_arg_elems;
                String_View *arg_elems = string_view_split(&arg_pair, '=', &num_arg_elems);

                String_View param = arg_elems[0];
                String_View value = arg_elems[1];

                if (string_view_equals_c_str(&param, "center_x"))
                {
                    center.x = strtof(value.c_str, NULL);
                }
                else if (string_view_equals_c_str(&param, "center_y"))
                {
                    center.y = strtof(value.c_str, NULL);
                }
                else if (string_view_equals_c_str(&param, "center_z"))
                {
                    center.z = strtof(value.c_str, NULL);
                }
                else if (string_view_equals_c_str(&param, "h_width"))
                {
                    extent.x = strtof(value.c_str, NULL);
                }
                else if (string_view_equals_c_str(&param, "h_height"))
                {
                    extent.y = strtof(value.c_str, NULL);
                }
                else if (string_view_equals_c_str(&param, "h_depth"))
                {
                    extent.z = strtof(value.c_str, NULL);
                }

                free(arg_elems);
            }

            Rect rect = { center, extent };
            vector_add(&rects, &rect);
        }
        else if (string_view_equals_c_str(&identifier, "player"))
        {
            for (uint32_t part = 1; part < num_parts; ++part)
            {
                String_View arg_pair = parts[part];

                uint32_t num_arg_elems;
                String_View *arg_elems = string_view_split(&arg_pair, '=', &num_arg_elems);

                String_View param = arg_elems[0];
                String_View value = arg_elems[1];

                if (string_view_equals_c_str(&param, "start_x"))
                {
                    start_position.x = strtof(value.c_str, NULL);
                }
                else if (string_view_equals_c_str(&param, "start_y"))
                {
                    start_position.y = strtof(value.c_str, NULL);
                }
                else if (string_view_equals_c_str(&param, "start_z"))
                {
                    start_position.z = strtof(value.c_str, NULL);
                }
                else if (string_view_equals_c_str(&param, "start_pitch"))
                {
                    start_pitch = strtof(value.c_str, NULL);
                }
                else if (string_view_equals_c_str(&param, "start_yaw"))
                {
                    start_yaw = strtof(value.c_str, NULL);
                }

                free(arg_elems);
            }
        }

        free(parts);
    }

    free(lines);
    string_destroy(&source_str);

    dest->start_position = start_position;
    dest->start_pitch = start_pitch;
    dest->start_yaw = start_yaw;

    Level_Data *data = malloc(sizeof(Level_Data));
    dest->data = data;

    data->num_rects = rects.size;
    data->rects = (Rect*)rects.arr;
    data->rect_index = 0;

    return IC_NO_ERROR;
}