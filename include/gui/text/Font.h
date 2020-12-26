#pragma once

#include"util/Vector.h"
#include"util/String.h"
#include"Core.h"

struct _Glyph
{
    char ascii;
    int32_t x, y, width, height;
    int32_t x_off, y_off;
    int32_t x_adv;
};

typedef struct _Glyph Glyph;

typedef struct _Font_Data Font_Data;

struct _Font
{
    Vector glyphs;

    uint32_t padding[4];
    uint32_t line_height;
    uint32_t width, height;
    uint32_t space_width;

    unsigned char *pixels;

    const Font_Data *data;
};

typedef struct _Font Font;

Font font_load(const char *name, IC_ERROR_CODE *error_code);
IC_ERROR_CODE font_create(Font *dest);
void font_destroy(const Font *font);

void font_bind(const Font *font);
Glyph *font_get_glyph(const Font *font, char c);