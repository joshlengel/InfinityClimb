#include"math/Color.h"

#include<glad/glad.h>

void context_init()
{
    glEnable(GL_DEPTH_TEST);
}

void context_terminate()
{

}

void context_background_color(const Color *color)
{
    glClearColor(color->red_f, color->green_f, color->blue_f, color->alpha_f);
}

void context_clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}