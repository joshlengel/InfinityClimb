#include"util/Color.h"

Color color_create_f(float red, float green, float blue, float alpha)
{
    Color res =
    {
        .red=(uint8_t)(red * 255.0f),
        .green=(uint8_t)(green * 255.0f),
        .blue=(uint8_t)(blue * 255.0f),
        .alpha=(uint8_t)(alpha * 255.0f),
        .red_f=red,
        .green_f=green,
        .blue_f=blue,
        .alpha_f=alpha
    };
    return res;
}

Color color_create_c(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
    Color res =
    {
        .red=red,
        .green=green,
        .blue=blue,
        .alpha=alpha,
        .red_f=(float)red / 255.0f,
        .green_f=(float)green / 255.0f,
        .blue_f=(float)blue / 255.0f,
        .alpha_f=(float)alpha / 255.0f
    };
    return res;
}

Color color_create_hex(uint32_t hex)
{
    Color res =
    {
        .red=(uint8_t)((hex >> 24) & 0xFF),
        .green=(uint8_t)((hex >> 16) & 0xFF),
        .blue=(uint8_t)((hex >> 8) & 0xFF),
        .alpha=(uint8_t)(hex & 0xFF)
    };
    
    res.red_f = (float)res.red / 255.0f;
    res.green_f = (float)res.green / 255.0f;
    res.blue_f = (float)res.blue / 255.0f;
    res.alpha_f = (float)res.alpha / 255.0f;

    return res;
}