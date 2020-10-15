#include"world/rect/Rect.h"

#include<glad/glad.h>
#include<stdlib.h>

struct Rect_Data
{
    GLuint vao_id;
    GLuint v_buff_id;
    GLuint i_buff_id;
};

IC_ERROR_CODE rect_create(Rect *dest)
{
    Rect_Data *data = malloc(sizeof(Rect_Data));
    glGenVertexArrays(1, &data->vao_id);
    glGenBuffers(1, &data->v_buff_id);
    glGenBuffers(1, &data->i_buff_id);

    dest->data = data;

    glBindVertexArray(data->vao_id);

    float vertices[] =
    { //       vertex                   normal
        -1.0f, -1.0f, -1.0f,      0.0f,  0.0f, -1.0f,
         1.0f, -1.0f, -1.0f,      0.0f,  0.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,      0.0f,  0.0f, -1.0f,
         1.0f,  1.0f, -1.0f,      0.0f,  0.0f, -1.0f,
         1.0f, -1.0f,  1.0f,      0.0f,  0.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,      0.0f,  0.0f,  1.0f,
         1.0f,  1.0f,  1.0f,      0.0f,  0.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,      0.0f,  0.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,     -1.0f,  0.0f,  0.0f,
        -1.0f, -1.0f, -1.0f,     -1.0f,  0.0f,  0.0f,
        -1.0f,  1.0f,  1.0f,     -1.0f,  0.0f,  0.0f,
        -1.0f,  1.0f, -1.0f,     -1.0f,  0.0f,  0.0f,
         1.0f, -1.0f, -1.0f,      1.0f,  0.0f,  0.0f,
         1.0f, -1.0f,  1.0f,      1.0f,  0.0f,  0.0f,
         1.0f,  1.0f, -1.0f,      1.0f,  0.0f,  0.0f,
         1.0f,  1.0f,  1.0f,      1.0f,  0.0f,  0.0f,
        -1.0f, -1.0f,  1.0f,      0.0f, -1.0f,  0.0f,
         1.0f, -1.0f,  1.0f,      0.0f, -1.0f,  0.0f,
        -1.0f, -1.0f, -1.0f,      0.0f, -1.0f,  0.0f,
         1.0f, -1.0f, -1.0f,      0.0f, -1.0f,  0.0f,
        -1.0f,  1.0f, -1.0f,      0.0f,  1.0f,  0.0f,
         1.0f,  1.0f, -1.0f,      0.0f,  1.0f,  0.0f,
        -1.0f,  1.0f,  1.0f,      0.0f,  1.0f,  0.0f,
         1.0f,  1.0f,  1.0f,      0.0f,  1.0f,  0.0f
    };

    GLsizei stride = sizeof(float) * 6; 

    glBindBuffer(GL_ARRAY_BUFFER, data->v_buff_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    uint8_t indices[] =
    {
        0, 1, 3,
        0, 3, 2,

        4, 5, 7,
        4, 7, 6,

        8, 9, 11,
        8, 11, 10,

        12, 13, 15,
        12, 15, 14,

        16, 17, 19,
        16, 19, 18,

        20, 21, 23,
        20, 23, 22
    };

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data->i_buff_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    return IC_NO_ERROR;
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