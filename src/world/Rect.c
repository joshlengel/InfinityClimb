#include"world/Rect.h"

#include<glad/glad.h>
#include<stdlib.h>

struct Rect_Data
{
    GLuint vao_id;
    GLuint v_buff_id;
    GLuint i_buff_id;
};

void rect_create(Rect *dest)
{
    Rect_Data *data = malloc(sizeof(Rect_Data));
    glGenVertexArrays(1, &data->vao_id);
    glGenBuffers(1, &data->v_buff_id);
    glGenBuffers(1, &data->i_buff_id);

    dest->data = data;

    glBindVertexArray(data->vao_id);

    float vertices[] =
    {
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f
    };

    glBindBuffer(GL_ARRAY_BUFFER, data->v_buff_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    uint8_t indices[] =
    {
        0, 1, 3,
        0, 3, 2,

        5, 4, 6,
        5, 6, 7,

        4, 0, 2,
        4, 2, 6,

        1, 5, 7,
        1, 7, 3,

        4, 5, 1,
        4, 1, 0,

        2, 3, 7,
        2, 7, 6
    };

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data->i_buff_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void rect_destroy(const Rect *rect)
{
    glDeleteVertexArrays(1, &rect->data->vao_id);
    glDeleteBuffers(1, &rect->data->v_buff_id);
    glDeleteBuffers(1, &rect->data->i_buff_id);

    free((void*)rect->data);
}

void rect_render(const Rect *rect)
{
    glBindVertexArray(rect->data->vao_id);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, NULL);
}