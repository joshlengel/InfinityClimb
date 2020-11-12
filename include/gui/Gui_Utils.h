#pragma once

#include"util/math/Vec.h"

#include<stdint.h>

typedef struct _Window Window;

float gui_width_to_extent(uint32_t width, const Window *window);
float gui_height_to_extent(uint32_t height, const Window *window);

Vec2 gui_px_to_coord(uint32_t left, uint32_t top, const Window *window);