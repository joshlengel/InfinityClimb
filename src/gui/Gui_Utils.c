#include"gui/Gui_Utils.h"

#include"window/Window.h"

float gui_width_to_extent(uint32_t width, const Window *window)
{
    return (float)width / (float)window->width;
}

float gui_height_to_extent(uint32_t height, const Window *window)
{
    return (float)height / (float)window->height;
}

Vec2 gui_px_to_coord(uint32_t left, uint32_t top, const Window *window)
{
    Vec2 res = { 2.0f * (float)left / (float)window->width - 1.0f, 1.0f - 2.0f * (float)top / (float)window->height };
    return res;
}