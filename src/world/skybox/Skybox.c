#include"world/skybox/Skybox.h"
#include"util/Utils.h"

#include<glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include<stb_image.h>

#include<stdlib.h>

struct _Skybox_Data
{
    GLuint vao_id;
    GLuint v_buff_id, i_buff_id;

    GLuint tex_id;
};

IC_ERROR_CODE skybox_create(Skybox *dest)
{
    Skybox_Data *data = malloc(sizeof(Skybox_Data));
    dest->data = data;

    glGenVertexArrays(1, &data->vao_id);
    glGenBuffers(1, &data->v_buff_id);
    glGenBuffers(1, &data->i_buff_id);

    glGenTextures(1, &data->tex_id);

    glBindVertexArray(data->vao_id);

    float vertices[] =
    { //       vertex
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

        5, 1, 2,
        5, 2, 6,

        1, 4, 7,
        1, 7, 3,

        4, 5, 1,
        4, 1, 0,

        2, 3, 7,
        2, 7, 6
    };

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data->i_buff_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    const char *tex_sources[6] =
    {
        dest->right_tex_path,
        dest->left_tex_path,
        dest->top_tex_path,
        dest->bottom_tex_path,
        dest->back_tex_path,
        dest->front_tex_path
    };

    glBindTexture(GL_TEXTURE_CUBE_MAP, data->tex_id);

    for (uint32_t i = 0; i < 6; ++i)
    {
        int width, height, comp;

        // Get pixels from texture file
        unsigned char *pixels = stbi_load(tex_sources[i], &width, &height, &comp, STBI_rgb_alpha);

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

        stbi_image_free(pixels);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return IC_NO_ERROR;
}

void skybox_destroy(const Skybox *skybox)
{
    glDeleteTextures(1, &skybox->data->tex_id);

    glDeleteBuffers(1, &skybox->data->v_buff_id);
    glDeleteBuffers(1, &skybox->data->i_buff_id);

    glDeleteVertexArrays(1, &skybox->data->vao_id);

    free((void*)skybox->data);
}

void skybox_render(const Skybox *skybox)
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->data->tex_id);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, NULL);
}