#pragma once

#include"Core.h"

#include"gui/text/Font.h"
#include"util/String.h"

typedef struct _Text_Data Text_Data;

struct _Text
{
    String text;
    const Font *font;
    float size;
    float line_width;

    const Text_Data *data;
};

typedef struct _Text Text;

IC_ERROR_CODE text_create(Text *dest);
void text_destroy(const Text *text);

void text_render(const Text *text);