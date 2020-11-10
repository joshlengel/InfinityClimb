#include"world/player/Crosshairs.h"

#include"world/model/Shader.h"
#include"util/Utils.h"
#include"window/Window.h"
#include"IC_Config.h"
#include"Log.h"

#include<glad/glad.h>
#include<stb_image.h>

const char *CROSSHAIRS_V_PATH = "assets/shaders/crosshairs.vert";
const char *CROSSHAIRS_F_PATH = "assets/shaders/crosshairs.frag";

struct _Crosshairs_Data
{
    GLuint vao_id;
    GLuint buff_id;

    GLuint tex_id;
    
    Shader shader;
};

IC_ERROR_CODE crosshairs_create(Crosshairs *dest)
{
    Crosshairs_Data *data = malloc(sizeof(Crosshairs_Data));
    log_assert(data != NULL, "Error creating crosshairs. Out of memory");
    dest->data = data;

    // Load vertex array
    glGenVertexArrays(1, &data->vao_id);
    glGenBuffers(1, &data->buff_id);

    glBindVertexArray(data->vao_id);
    glBindBuffer(GL_ARRAY_BUFFER, data->buff_id);

    const float vertices[] =
    { //   vertex          texture coords
        -1.0f, -1.0f,        0.0f, 1.0f,
         1.0f, -1.0f,        1.0f, 1.0f,
         1.0f,  1.0f,        1.0f, 0.0f,
        -1.0f, -1.0f,        0.0f, 1.0f,
         1.0f,  1.0f,        1.0f, 0.0f,
        -1.0f,  1.0f,        0.0f, 0.0f
    };
    GLsizei stride = 4 * sizeof(float);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // Load texture
    String_View rel_path;
    string_view_create_c_str(&rel_path, dest->texture_path, 0, UINT32_MAX);

    String_View prefix;
    string_view_create_c_str(&prefix, IC_WORKING_DIRECTORY, 0, UINT32_MAX);

    String path = string_concat_sv(&prefix, &rel_path);

    int width, height, comp;
    stbi_uc *pixels = stbi_load(path.c_str, &width, &height, &comp, STBI_rgb_alpha);

    string_destroy(&path);

    if (pixels == NULL)
    {
        log_trace("Error loading crosshairs texture at '%s'. STBI failure reason: %s", path.c_str, stbi_failure_reason());
        glDeleteVertexArrays(1, &data->vao_id);
        glDeleteBuffers(1, &data->buff_id);
        free(data);
        return IC_READ_ERROR;
    }

    glGenTextures(1, &data->tex_id);
    glBindTexture(GL_TEXTURE_2D, data->tex_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)width, (GLsizei)height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(pixels);

    // Load shader
    IC_ERROR_CODE ec;

    String v_src = read_source(CROSSHAIRS_V_PATH, &ec);
    if (ec != IC_NO_ERROR)
    {
        glDeleteVertexArrays(1, &data->vao_id);
        glDeleteBuffers(1, &data->buff_id);
        glDeleteTextures(1, &data->tex_id);
        free(data);
        return ec;
    }

    String f_src = read_source(CROSSHAIRS_F_PATH, &ec);
    if (ec != IC_NO_ERROR)
    {
        string_destroy(&v_src);
        glDeleteVertexArrays(1, &data->vao_id);
        glDeleteBuffers(1, &data->buff_id);
        glDeleteTextures(1, &data->tex_id);
        free(data);
        return ec;
    }

    string_view_create_s(&data->shader.vertex_source, &v_src, 0, UINT32_MAX);
    string_view_create_s(&data->shader.fragment_source, &f_src, 0, UINT32_MAX);

    data->shader.num_uniforms = 2;

    ec = shader_create(&data->shader);
    if (ec != IC_NO_ERROR)
    {
        string_destroy(&v_src);
        string_destroy(&f_src);
        glDeleteVertexArrays(1, &data->vao_id);
        glDeleteBuffers(1, &data->buff_id);
        glDeleteTextures(1, &data->tex_id);
        free(data);
        return ec;
    }

    string_destroy(&v_src);
    string_destroy(&f_src);

    shader_declare_uniform(&data->shader, "scale");
    shader_declare_uniform(&data->shader, "aspect_ratio");
}

void crosshairs_destroy(const Crosshairs *crosshairs)
{
    glDeleteVertexArrays(1, &crosshairs->data->vao_id);
    glDeleteBuffers(1, &crosshairs->data->buff_id);

    glDeleteTextures(1, &crosshairs->data->tex_id);

    shader_destroy(&crosshairs->data->shader);
    free((void*)crosshairs->data);
}

void crosshairs_render(const Crosshairs *crosshairs, const Window *window)
{
    shader_bind(&crosshairs->data->shader);
    shader_set_uniform_1f(&crosshairs->data->shader, "scale", crosshairs->scale);
    shader_set_uniform_1f(&crosshairs->data->shader, "aspect_ratio", window_aspect_ratio(window));

    glBindVertexArray(crosshairs->data->vao_id);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, crosshairs->data->tex_id);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}