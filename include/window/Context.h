#pragma once

#include"Utils.h"

typedef struct _Color Color;

void context_init();
void context_background_color(const Color *color);
void context_clear();