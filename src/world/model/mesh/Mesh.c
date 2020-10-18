#include"world/model/mesh/Mesh.h"

#include<glad/glad.h>

#include<stdlib.h>

struct _Mesh_Data
{
    GLuint vao_id;
    GLuint v_buff_id;
    GLuint i_buff_id;

    uint32_t num_indices;
};

IC_ERROR_CODE mesh_create(Mesh *dest)
{
    Mesh_Data *data = malloc(sizeof(Mesh_Data));
    dest->data = data;
    data->num_indices = dest->num_indices;

    glGenVertexArrays(1, &data->vao_id);
    glGenBuffers(1, &data->v_buff_id);
    glGenBuffers(1, &data->i_buff_id);

    glBindVertexArray(data->vao_id);

    uint32_t num_elems = 0;
    if (dest->vertices) num_elems += 3;
    if (dest->texture_coords) num_elems += 2;
    if (dest->normals) num_elems += 3;
    
    GLsizei stride = num_elems * sizeof(float);
    uint32_t arr_size = dest->num_vertices * stride;
    float *arr = malloc(arr_size);
    float *itr = arr;

    const float *v_itr = dest->vertices;
    const float *tc_itr = dest->texture_coords;
    const float *n_itr = dest->normals;

    for (uint32_t i = 0; i < dest->num_vertices; ++i)
    {
        if (dest->vertices)
        {
            *(itr++) = *(v_itr++);
            *(itr++) = *(v_itr++);
            *(itr++) = *(v_itr++);
        }

        if (dest->texture_coords)
        {
            *(itr++) = *(tc_itr++);
            *(itr++) = *(tc_itr++);
        }

        if (dest->normals)
        {
            *(itr++) = *(n_itr++);
            *(itr++) = *(n_itr++);
            *(itr++) = *(n_itr++);
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, data->v_buff_id);
    glBufferData(GL_ARRAY_BUFFER, arr_size, arr, GL_STATIC_DRAW);

    free(arr);

    GLuint index = 0;
    void *pointer = (void*)0;

    if (dest->vertices)
    {
        glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, stride, pointer);
        glEnableVertexAttribArray(index);
        pointer += 3 * sizeof(float);
        ++index;
    }

    if (dest->texture_coords)
    {
        glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, stride, pointer);
        glEnableVertexAttribArray(index);
        pointer += 2 * sizeof(float);
        ++index;
    }

    if (dest->normals)
    {
        glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, stride, pointer);
        glEnableVertexAttribArray(index);
        pointer += 3 * sizeof(float);
        ++index;
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data->i_buff_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, dest->num_indices * sizeof(uint32_t), dest->indices, GL_STATIC_DRAW);

    return IC_NO_ERROR;
}

void mesh_destroy(const Mesh *mesh)
{
    glDeleteBuffers(1, &mesh->data->v_buff_id);
    glDeleteBuffers(1, &mesh->data->i_buff_id);
    glDeleteVertexArrays(1, &mesh->data->vao_id);

    free((void*)mesh->data);
}

void mesh_render(const Mesh *mesh)
{
    glBindVertexArray(mesh->data->vao_id);
    glDrawElements(GL_TRIANGLES, (GLsizei)mesh->data->num_indices, GL_UNSIGNED_INT, NULL);
}