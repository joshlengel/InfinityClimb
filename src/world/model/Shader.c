#include"world/model/Shader.h"

#include"Log.h"

#include<glad/glad.h>

#include<stdlib.h>
#include<assert.h>
#include<string.h>

struct _Uniform_Location_Pair
{
    const char *name;
    GLint location;  
};

typedef struct _Uniform_Location_Pair Uniform_Location_Pair;

struct _Shader_Data
{
    GLuint program_id;
    GLuint v_shader_id;
    GLuint f_shader_id;

    Uniform_Location_Pair *uniform_locations;
    uint32_t uniform_index;
    uint32_t num_uniforms;
};

IC_ERROR_CODE shader_create(Shader *dest)
{
    Shader_Data *data = malloc(sizeof(Shader_Data));
    log_assert(data != NULL, "Error creating shader. Out of memory");
    dest->data = data;

    data->program_id = glCreateProgram();
    data->v_shader_id = glCreateShader(GL_VERTEX_SHADER);
    data->f_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    data->uniform_locations = malloc(sizeof(Uniform_Location_Pair) * dest->num_uniforms);
    log_assert(data != NULL, "Error creating shader. Out of memory");
    data->uniform_index = 0;
    data->num_uniforms = dest->num_uniforms; // In case user changes this number

    GLint v_src_length = (GLint)dest->vertex_source.length;
    GLint f_src_length = (GLint)dest->fragment_source.length; 

    const char *v_src = dest->vertex_source.c_str;
    const char *f_src = dest->fragment_source.c_str;

    glShaderSource(data->v_shader_id, 1, &v_src, &v_src_length);
    glShaderSource(data->f_shader_id, 1, &f_src, &f_src_length);

    glCompileShader(data->v_shader_id);
    glCompileShader(data->f_shader_id);

    GLint comp_status;

    glGetShaderiv(data->v_shader_id, GL_COMPILE_STATUS, &comp_status);
    if (comp_status == GL_FALSE)
    {
        GLint log_length;
        glGetShaderiv(data->v_shader_id, GL_INFO_LOG_LENGTH, &log_length);

        char *msg = malloc(sizeof(char) * log_length);
        glGetShaderInfoLog(data->v_shader_id, log_length, NULL, msg);

        log_trace("Error compiling vertex shader. Info log:\n%s", msg);
        free(msg);

        shader_destroy(dest);
        return IC_SHADER_COMPILE_ERROR;
    }

    glGetShaderiv(data->f_shader_id, GL_COMPILE_STATUS, &comp_status);
    if (comp_status == GL_FALSE)
    {
        GLint log_length;
        glGetShaderiv(data->f_shader_id, GL_INFO_LOG_LENGTH, &log_length);

        char *msg = malloc(sizeof(char) * log_length);
        glGetShaderInfoLog(data->f_shader_id, log_length, NULL, msg);

        log_trace("Error compiling fragment shader. Info log:\n%s", msg);
        free(msg);

        shader_destroy(dest);
        return IC_SHADER_COMPILE_ERROR;
    }

    glAttachShader(data->program_id, data->v_shader_id);
    glAttachShader(data->program_id, data->f_shader_id);

    glLinkProgram(data->program_id);

    GLint link_status;

    glGetProgramiv(data->program_id, GL_LINK_STATUS, &link_status);
    if (link_status == GL_FALSE)
    {
        GLint log_length;
        glGetProgramiv(data->program_id, GL_INFO_LOG_LENGTH, &log_length);

        char *msg = malloc(sizeof(char) * log_length);
        glGetProgramInfoLog(data->program_id, log_length, NULL, msg);

        log_trace("Error linking shader program. Info log:\n%s", msg);
        free(msg);

        shader_destroy(dest);
        return IC_SHADER_LINK_ERROR;
    }

    return IC_NO_ERROR;
}

void shader_destroy(const Shader *shader)
{
    glDetachShader(shader->data->program_id, shader->data->v_shader_id);
    glDetachShader(shader->data->program_id, shader->data->f_shader_id);

    glDeleteShader(shader->data->v_shader_id);
    glDeleteShader(shader->data->f_shader_id);

    glDeleteProgram(shader->data->program_id);

    free(shader->data->uniform_locations);
    free((void*)shader->data);
}

void shader_declare_uniform(const Shader *shader, const char *name)
{
    assert(shader->data->uniform_index < shader->data->num_uniforms);

    Uniform_Location_Pair *pair = shader->data->uniform_locations + shader->data->uniform_index++;
    pair->name = name;
    pair->location = glGetUniformLocation(shader->data->program_id, name);
}

// Utility function
static GLint __get_uniform_location_impl(const Shader *shader, const char *name)
{
    Uniform_Location_Pair *pair = shader->data->uniform_locations;

    for (uint32_t i = 0; i < shader->data->uniform_index; ++i)
    {
        if (strcmp(pair->name, name) == 0)
        {
            return pair->location;
        }

        ++pair;
    }

    assert(0);
    return -1;
}

void shader_set_uniform_1i(const Shader *shader, const char *name, int32_t v)
{
    glUniform1i(__get_uniform_location_impl(shader, name), v);
}

void shader_set_uniform_2i(const Shader *shader, const char *name, int32_t v1, int32_t v2)
{
    glUniform2i(__get_uniform_location_impl(shader, name), v1, v2);
}

void shader_set_uniform_3i(const Shader *shader, const char *name, int32_t v1, int32_t v2, int32_t v3)
{
    glUniform3i(__get_uniform_location_impl(shader, name), v1, v2, v3);
}

void shader_set_uniform_4i(const Shader *shader, const char *name, int32_t v1, int32_t v2, int32_t v3, int32_t v4)
{
    glUniform4i(__get_uniform_location_impl(shader, name), v1, v2, v3, v4);
}

void shader_set_uniform_1f(const Shader *shader, const char *name, float v)
{
    glUniform1f(__get_uniform_location_impl(shader, name), v);
}

void shader_set_uniform_2f(const Shader *shader, const char *name, float v1, float v2)
{
    glUniform2f(__get_uniform_location_impl(shader, name), v1, v2);
}

void shader_set_uniform_3f(const Shader *shader, const char *name, float v1, float v2, float v3)
{
    glUniform3f(__get_uniform_location_impl(shader, name), v1, v2, v3);
}

void shader_set_uniform_4f(const Shader *shader, const char *name, float v1, float v2, float v3, float v4)
{
    glUniform4f(__get_uniform_location_impl(shader, name), v1, v2, v3, v4);
}

void shader_set_uniform_mat3(const Shader *shader, const char *name, float *values)
{
    glUniformMatrix3fv(__get_uniform_location_impl(shader, name), 1, GL_TRUE, values);
}

void shader_set_uniform_mat4(const Shader *shader, const char *name, float *values)
{
    glUniformMatrix4fv(__get_uniform_location_impl(shader, name), 1, GL_TRUE, values);
}

void shader_bind(const Shader *shader)
{
    glUseProgram(shader->data->program_id);
}