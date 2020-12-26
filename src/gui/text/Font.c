#include"gui/text/Font.h"
#include"Log.h"

#include"util/Utils.h"
#include"IC_Config.h"

#include<glad/glad.h>
#include<stb_image.h>

struct _Font_Data
{
    GLuint tex_id;
};

static const char *const FONT_PREFIX = "assets/fonts/";
static const char *const FONT_FNT = ".fnt";
static const char *const FONT_PNG = ".png";

Font font_load(const char *name, IC_ERROR_CODE *error_code)
{
    if (error_code) *error_code = IC_NO_ERROR;
    Font res = {0};

    // Initialize path segments
    String_View font_prefix;
    string_view_create_c_str(&font_prefix, FONT_PREFIX, 0, UINT32_MAX);

    String_View font_name;
    string_view_create_c_str(&font_name, name, 0, UINT32_MAX);

    String_View font_fnt;
    string_view_create_c_str(&font_fnt, FONT_FNT, 0, UINT32_MAX);

    String_View font_png;
    string_view_create_c_str(&font_png, FONT_PNG, 0, UINT32_MAX);

    String fnt_path_name_str = string_concat_sv(&font_prefix, &font_name);
    String_View fnt_path_name;
    string_view_create_s(&fnt_path_name, &fnt_path_name_str, 0, UINT32_MAX);

    String fnt_path = string_concat_sv(&fnt_path_name, &font_fnt);
    String tex_path = string_concat_sv(&fnt_path_name, &font_png);

    IC_ERROR_CODE ec;
    String source_str = read_source(fnt_path.c_str, &ec);

    string_destroy(&fnt_path_name_str);
    
    if (ec != IC_NO_ERROR)
    {
        string_destroy(&fnt_path);
        string_destroy(&tex_path);
        if (error_code) *error_code = ec;
        return res;
    }

    res.glyphs.elem_size = sizeof(Glyph);
    res.glyphs.init_capacity = 26;

    if ((ec = vector_create(&res.glyphs)) != IC_NO_ERROR)
    {
        string_destroy(&fnt_path);
        string_destroy(&tex_path);
        if (error_code) *error_code = ec;
        return res;
    }

    String_View source;
    string_view_create_s(&source, &source_str, 0, UINT32_MAX);

    uint32_t num_lines;
    String_View *lines = string_view_split(&source, '\n', &num_lines);

    for (uint32_t l = 0; l < num_lines; ++l)
    {
        String_View line = string_view_trim(&lines[l]);

        uint32_t num_parts;
        String_View *parts = string_view_split(&line, ' ', &num_parts);

        if (num_parts == 0)
        {
            free(parts);
            log_trace("Warning on line %u at '%s': Empty line with whitespace found. This is not critical, but leads to inefficiencies", l + 1, fnt_path.c_str);
            if (error_code) *error_code = IC_FONT_WHITESPACE_INEFFICIENCY;
            continue;
        }

        if (string_view_equals_c_str(&parts[0], "info"))
        {
            for (uint32_t i = 1; i < num_parts; ++i)
            {
                uint32_t num_arg_pair;
                String_View *arg_pair = string_view_split(&parts[i], '=', &num_arg_pair);

                if (string_view_equals_c_str(&arg_pair[0], "padding"))
                {
                    uint32_t num_elements;
                    String_View *elements = string_view_split(&arg_pair[1], ',', &num_elements);

                    for (uint32_t j = 0; j < 4; ++j)
                    {
                        res.padding[j] = (uint32_t)strtoul(elements[j].c_str, NULL, 10);
                    }

                    free(elements);
                }

                free(arg_pair);
            }
        }
        else if (string_view_equals_c_str(&parts[0], "common"))
        {
            for (uint32_t i = 1; i < num_parts; ++i)
            {
                uint32_t num_arg_pair;
                String_View *arg_pair = string_view_split(&parts[i], '=', &num_arg_pair);

                if (string_view_equals_c_str(&arg_pair[0], "lineHeight"))
                {
                    res.line_height = (uint32_t)strtoul(arg_pair[1].c_str, NULL, 10);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "scaleW"))
                {
                    res.width = (uint32_t)strtoul(arg_pair[1].c_str, NULL, 10);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "scaleH"))
                {
                    res.height = (uint32_t)strtoul(arg_pair[1].c_str, NULL, 10);
                }

                free(arg_pair);
            }
        }
        else if (string_view_equals_c_str(&parts[0], "char"))
        {
            Glyph g = {0};

            for (uint32_t i = 1; i < num_parts; ++i)
            {
                uint32_t num_arg_pair;
                String_View *arg_pair = string_view_split(&parts[i], '=', &num_arg_pair);

                if (string_view_equals_c_str(&arg_pair[0], "id"))
                {
                    g.ascii = (char)strtoul(arg_pair[1].c_str, NULL, 10);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "x"))
                {
                    g.x = (int32_t)strtol(arg_pair[1].c_str, NULL, 10);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "y"))
                {
                    g.y = (int32_t)strtol(arg_pair[1].c_str, NULL, 10);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "width"))
                {
                    g.width = (int32_t)strtol(arg_pair[1].c_str, NULL, 10);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "height"))
                {
                    g.height = (int32_t)strtol(arg_pair[1].c_str, NULL, 10);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "xoffset"))
                {
                    g.x_off = (int32_t)strtol(arg_pair[1].c_str, NULL, 10);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "yoffset"))
                {
                    g.y_off = (int32_t)strtol(arg_pair[1].c_str, NULL, 10);
                }
                else if (string_view_equals_c_str(&arg_pair[0], "xadvance"))
                {
                    g.x_adv = (int32_t)strtol(arg_pair[1].c_str, NULL, 10);
                }

                free(arg_pair);
            }

            if (g.ascii == ' ')
                res.space_width = (uint32_t)g.x_adv;
            else
                vector_add(&res.glyphs, &g);
        }

        free(parts);
    }

    free(lines);

    string_destroy(&source_str);

    string_destroy(&fnt_path);

    String_View work_dir;
    String_View tex_rel_path_sv;
    string_view_create_c_str(&work_dir, IC_WORKING_DIRECTORY, 0, UINT32_MAX);
    string_view_create_s(&tex_rel_path_sv, &tex_path, 0, UINT32_MAX);

    String tex_abs_path = string_concat_sv(&work_dir, &tex_rel_path_sv);

    log_assert(sizeof(unsigned char) == sizeof(stbi_uc), "Error. Image loading library doesn't use 32-bit pixels");
    int width, height, comp;
    res.pixels = (unsigned char*)stbi_load(tex_abs_path.c_str, &width, &height, &comp, STBI_rgb_alpha);
    res.width = (uint32_t)width;
    res.height = (uint32_t)height;

    string_destroy(&tex_path);
    string_destroy(&tex_abs_path);

    return res;
}

IC_ERROR_CODE font_create(Font *dest)
{
    Font_Data *data = malloc(sizeof(Font_Data));
    log_assert(data != NULL, "Error creating font. Out of memory");
    dest->data = data;

    glGenTextures(1, &data->tex_id);
    glBindTexture(GL_TEXTURE_2D, data->tex_id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)dest->width, (GLsizei)dest->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dest->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(dest->pixels);
}

void font_destroy(const Font *font)
{
    vector_destroy(&font->glyphs);

    glDeleteTextures(1, &font->data->tex_id);
    free((void*)font->data);
}

void font_bind(const Font *font)
{
    glBindTexture(GL_TEXTURE_2D, font->data->tex_id);
}

Glyph *font_get_glyph(const Font *font, char c)
{
    Glyph *itr = (Glyph*)font->glyphs.arr;

    for (uint32_t i = 0; i < font->glyphs.size; ++i)
    {
        if (itr->ascii == c) return itr;
        ++itr;
    }

    return NULL;
}