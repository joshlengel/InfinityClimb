#pragma once

#include"Core.h"

#include<stdint.h>

struct _Color
{
    uint8_t red, green, blue, alpha;
    float red_f, green_f, blue_f, alpha_f;
};

typedef struct _Color Color;

Color color_create_f(float red, float green, float blue, float alpha);
Color color_create_c(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
Color color_create_hex(uint32_t hex);