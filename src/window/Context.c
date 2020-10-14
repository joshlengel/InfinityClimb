#include<glad/glad.h>

void context_init()
{
    glEnable(GL_DEPTH_TEST);
}

void context_terminate()
{

}

void context_background_color(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}

void context_clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}