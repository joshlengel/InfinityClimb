#include"window/Context.h"
#include"util/Color.h"
#include"Log.h"

#include<glad/glad.h>

#include<stdlib.h>

static const Color DEFAULT = { .red_f=0.0f, .green_f=0.0f, .blue_f=0.0f, .alpha_f=1.0f };

struct _Context_Data
{
    GLbitfield clear_mask;
};

IC_ERROR_CODE context_create(Context *dest)
{
    Context_Data *data = malloc(sizeof(Context_Data));
    dest->data = data;
    log_assert(data != NULL, "Error creating context. Out of memory");

    context_update(dest);

    return IC_NO_ERROR;
}

void context_destroy(const Context *context)
{
    free(context->data);
}

void context_update(const Context *context)
{
    context->data->clear_mask = 0;

    if (context->clear_color) context->data->clear_mask |= GL_COLOR_BUFFER_BIT;
    if (context->clear_depth) context->data->clear_mask |= GL_DEPTH_BUFFER_BIT;

    const Color *clear_color = context->background_color? context->background_color : &DEFAULT;

    glClearColor(
        clear_color->red_f,
        clear_color->green_f,
        clear_color->blue_f,
        clear_color->alpha_f);
    
    if (context->cull)
    {
        glEnable(GL_CULL_FACE);

        if (context->cull_front)
            glCullFace(GL_FRONT);
        else
            glCullFace(GL_BACK);
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }
    
    if (context->depth_test)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
    
    if (context->blending)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else
    {
        glDisable(GL_BLEND);
    }
}

void context_clear(const Context *context)
{
    glClear(context->data->clear_mask);
}