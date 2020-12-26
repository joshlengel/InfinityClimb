#include"gui/text/Text.h"

#include"util/Vector.h"
#include"Log.h"

#include<glad/glad.h>
#include<stdlib.h>

struct _Text_Data
{
    GLuint vao_id;
    GLuint v_buff_id;
    GLuint i_buff_id;

    uint32_t num_indices;
};

IC_ERROR_CODE text_create(Text *dest)
{
    log_assert(dest->font != NULL, "Error creating text. Font must not be null");

    Text_Data *data = malloc(sizeof(Text_Data));
    log_assert(data != NULL, "Error creating text. Out of memory");

    dest->data = data;
    
    // Load vertex buffers and calculate vertices and texture coordinates
    IC_ERROR_CODE ec = IC_NO_ERROR;
    
    uint32_t vertex_size = sizeof(float) * (2 + 2);
    Vector vertices = { .init_capacity=100, .elem_size=sizeof(float) };
    ec = vector_create(&vertices);
    
    if (ec != IC_NO_ERROR)
    {
        free(data);
        return ec;
    }

    Vector indices = { .init_capacity=150, .elem_size=sizeof(uint32_t) };
    ec = vector_create(&indices);
    
    if (ec != IC_NO_ERROR)
    {
        vector_destroy(&vertices);
        free(data);
        return ec;
    }

    uint32_t index = 0;

    const char *itr = dest->text.c_str;

    float cx = 0.0f;
    float cy = 0.0f;

    const float t_sx = 1.0f / dest->font->width;
    const float t_sy = 1.0f / dest->font->height;

    const float left_padding = dest->font->padding[3] * t_sx;
    const float top_padding = dest->font->padding[0] * t_sy;
    const float horiz_padding = (dest->font->padding[1] + dest->font->padding[3]) * t_sx;
    const float vert_padding = (dest->font->padding[0] + dest->font->padding[2]) * t_sy;

    const float line_height = dest->font->line_height * t_sy - vert_padding;
    const float space_width = dest->font->space_width * t_sx;

    for (uint32_t i = 0; i < dest->text.length; ++i)
    {
        if (*itr == ' ')
        {
            cx += space_width;
            ++itr;
            continue;
        }

        Glyph *g = font_get_glyph(dest->font, *itr);
        log_assert(g != NULL, "Error creating text. Font data for character '%c' not found", *itr);

        float tx = g->x * t_sx;
        float ty = g->y * t_sy;
        float w = g->width * t_sx;
        float h = g->height * t_sy;

        float x = cx - left_padding + g->x_off * t_sx;
        float y = cy + top_padding - g->y_off * t_sy;

        float x1 = x;
        float x2 = x + w;
        float y1 = y - h;
        float y2 = y;

        float tx1 = tx;
        float tx2 = tx + w;
        float ty1 = ty + h;
        float ty2 = ty;

        // vertex (bottom left)
        vector_add(&vertices, &x1);
        vector_add(&vertices, &y1);

        // texture coord (bottom left)
        vector_add(&vertices, &tx1);
        vector_add(&vertices, &ty1);

        // vertex (bottom right)
        vector_add(&vertices, &x2);
        vector_add(&vertices, &y1);

        // texture coord (bottom left)
        vector_add(&vertices, &tx2);
        vector_add(&vertices, &ty1);

        // vertex (top left)
        vector_add(&vertices, &x1);
        vector_add(&vertices, &y2);

        // texture coord (top left)
        vector_add(&vertices, &tx1);
        vector_add(&vertices, &ty2);

        // vertex (top right)
        vector_add(&vertices, &x2);
        vector_add(&vertices, &y2);

        // texture coord (top right)
        vector_add(&vertices, &tx2);
        vector_add(&vertices, &ty2);

        uint32_t i1 = index;
        uint32_t i2 = index + 1;
        uint32_t i3 = index + 2;
        uint32_t i4 = index + 3;

        // indices
        vector_add(&indices, &i1);
        vector_add(&indices, &i2);
        vector_add(&indices, &i4);

        vector_add(&indices, &i1);
        vector_add(&indices, &i4);
        vector_add(&indices, &i3);

        index += 4;

        cx += g->x_adv * t_sx - horiz_padding;

        if (cx > dest->line_width)
        {
            cx = 0.0f;
            cy -= line_height;
        }

        ++itr;
    }

    data->num_indices = indices.size;

    glGenVertexArrays(1, &data->vao_id);
    glGenBuffers(1, &data->v_buff_id);
    glGenBuffers(1, &data->i_buff_id);

    glBindVertexArray(data->vao_id);
    
    glBindBuffer(GL_ARRAY_BUFFER, data->v_buff_id);
    glBufferData(GL_ARRAY_BUFFER, vertices.elem_size * vertices.size, vertices.arr, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, vertex_size, (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vertex_size, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data->i_buff_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.elem_size * indices.size, indices.arr, GL_STATIC_DRAW);

    vector_destroy(&vertices);
    vector_destroy(&indices);
}

void text_destroy(const Text *text)
{
    glDeleteVertexArrays(1, &text->data->vao_id);
    glDeleteBuffers(1, &text->data->v_buff_id);
    glDeleteBuffers(1, &text->data->i_buff_id);

    free((void*)text->data);
}

void text_render(const Text *text)
{
    font_bind(text->font);
    glBindVertexArray(text->data->vao_id);
    glDrawElements(GL_TRIANGLES, text->data->num_indices, GL_UNSIGNED_INT, NULL);
}