#include"world/model/mesh/Mesh.h"

#include"util/math/Vec.h"
#include"util/String.h"
#include"util/Vector.h"
#include"util/Utils.h"
#include"Log.h"

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
    log_assert(data != NULL, "Error creating mesh. Out of memory");

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
    if (arr == NULL)
    {
        log_trace("Error creating mesh. Out of memory");
        mesh_destroy(dest);
        return IC_OUT_OF_MEMORY_ERROR;
    }

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
    int8_t *pointer = (int8_t*)0;

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

Mesh mesh_load_from_obj(const char *path, IC_ERROR_CODE *error_code)
{
    Mesh res = {0};

    if (error_code) *error_code = IC_NO_ERROR;

    IC_ERROR_CODE ec;
    String source_str = read_source(path, &ec);
    if (ec != IC_NO_ERROR)
    {
        if (error_code) *error_code = ec;
        return res;
    }

    String_View source;
    string_view_create_s(&source, &source_str, 0, UINT32_MAX);

    typedef struct _Vertex Vertex;

    struct _Vertex
    {
        Vec3 position;
        uint32_t tex_index;
        uint32_t norm_index;
        uint32_t index;
        IC_BOOL set;

        Vertex *duplicate;
    };

    Vector vertices = { .elem_size=sizeof(Vertex), .init_capacity=50 };
    Vector texture_coords = { .elem_size=sizeof(Vec2), .init_capacity=50 };
    Vector normals = { .elem_size=sizeof(Vec3), .init_capacity=50 };

    ec = vector_create(&vertices);
    if (ec != IC_NO_ERROR)
    {
        string_destroy(&source_str);
        if (error_code) *error_code = ec;
        return res;
    }

    ec = vector_create(&texture_coords);
    if (ec != IC_NO_ERROR)
    {
        string_destroy(&source_str);
        vector_destroy(&vertices);
        if (error_code) *error_code = ec;
        return res;
    }

    ec = vector_create(&normals);
    if (ec != IC_NO_ERROR)
    {
        string_destroy(&source_str);
        vector_destroy(&vertices);
        vector_destroy(&texture_coords);
        if (error_code) *error_code = ec;
        return res;
    }

    IC_BOOL use_texture_coords = IC_FALSE;
    IC_BOOL use_normals = IC_FALSE;

    uint32_t num_lines;
    String_View *lines = string_view_split(&source, '\n', &num_lines);
    uint32_t l = 0;

    for (; l < num_lines; ++l)
    {
        String_View line = lines[l];
        line = string_view_trim(&line);

        uint32_t num_parts;
        String_View *parts = string_view_split(&line, ' ', &num_parts);

        if (string_view_equals_c_str(&parts[0], "v"))
        {
            Vertex v = {
                .position={
                    strtof(parts[1].c_str, NULL),
                    strtof(parts[2].c_str, NULL),
                    strtof(parts[3].c_str, NULL)
                },
                .index=vertices.size,
                .set=IC_FALSE,
                .duplicate=NULL
            };

            vector_add(&vertices, &v);
        }
        else if (string_view_equals_c_str(&parts[0], "vt"))
        {
            use_texture_coords = IC_TRUE;

            Vec2 vt = {
                strtof(parts[1].c_str, NULL),
                strtof(parts[2].c_str, NULL)
            };

            vector_add(&texture_coords, &vt);
        }
        else if (string_view_equals_c_str(&parts[0], "vn"))
        {
            use_normals = IC_TRUE;

            Vec3 vn = {
                strtof(parts[1].c_str, NULL),
                strtof(parts[2].c_str, NULL),
                strtof(parts[3].c_str, NULL)
            };

            vector_add(&normals, &vn);
        }
        else if (string_view_equals_c_str(&parts[0], "f"))
        {
            free(parts);
            break;
        }
        else if (!string_view_equals_c_str(&parts[0], "#") && !string_view_equals_c_str(&parts[0], "o"))
        {
            log_trace("Warning on line %u at '%s': Unrecognized identifier '%.*s' for object", l + 1, path, parts[0].length, parts[0].c_str);
        }

        free(parts);
    }

    Vector indices = { .elem_size=sizeof(uint32_t), .init_capacity=50 };
    ec = vector_create(&indices);
    if (ec != IC_NO_ERROR)
    {
        string_destroy(&source_str);
        vector_destroy(&vertices);
        vector_destroy(&texture_coords);
        vector_destroy(&normals);
        free(lines);
        if (error_code) *error_code = ec;
        return res;
    }

    for (; l < num_lines; ++l)
    {
        String_View line = lines[l];
        line = string_view_trim(&line);

        uint32_t num_parts;
        String_View *parts = string_view_split(&line, ' ', &num_parts);

        if (string_view_equals_c_str(&parts[0], "f"))
        {
            for (uint32_t i = 1; i <= 3; ++i)
            {
                uint32_t num_index_parts;
                String_View *index_parts = string_view_split(&parts[i], '/', &num_index_parts);

                uint32_t v_index = (uint32_t)strtoul(index_parts[0].c_str, NULL, 10) - 1;
                uint32_t tc_index = 0;
                uint32_t n_index = 0;

                uint32_t index_part_i = 1;
                if (use_texture_coords)
                {
                    tc_index = (uint32_t)strtoul(index_parts[index_part_i++].c_str, NULL, 10) - 1;
                }

                if (use_normals)
                {
                    n_index = (uint32_t)strtoul(index_parts[index_part_i++].c_str, NULL, 10) - 1;
                }

                Vertex *vertex = (Vertex*)vector_get(&vertices, v_index);
                if (vertex->set)
                {
                    if (vertex->tex_index == tc_index && vertex->norm_index == n_index)
                    {
                        vector_add(&indices, &vertex->index);
                    }
                    else
                    {
                        Vertex *v = vertex;

                        while (v->duplicate)
                        {
                            if (v->duplicate->set && v->duplicate->tex_index == tc_index && v->duplicate->norm_index == n_index)
                            {
                                vector_add(&indices, &v->duplicate->index);
                                break;
                            }

                            v = v->duplicate;
                        }

                        if (!v->duplicate)
                        {
                            Vertex dup = {
                                .position=v->position,
                                .tex_index=tc_index,
                                .norm_index=n_index,
                                .index=vertices.size,
                                .set=IC_TRUE,
                                .duplicate=NULL
                            };

                            vector_add(&indices, &dup.index);
                            v->duplicate = (Vertex*)vector_add_r(&vertices, &dup);
                        }
                    }
                }
                else
                {
                    vertex->tex_index = tc_index;
                    vertex->norm_index = n_index;
                    vertex->set = IC_TRUE;
                    vector_add(&indices, &vertex->index);
                }

                free(index_parts);
            }
        }
        else
        {
            log_trace("Warning on line %u at '%s': Unrecognized identifier '%.*s' for object", l + 1, path, parts[0].length, parts[0].c_str);
        }

        free(parts);
    }

    free(lines);
    string_destroy(&source_str);

    float *vertices_arr = NULL;
    float *texture_coords_arr = NULL;
    float *normals_arr = NULL;

    vertices_arr = malloc(sizeof(float) * vertices.size * 3);
    log_assert(vertices_arr != NULL, "Error creating mesh. Out of memory");
    
    if (use_texture_coords)
    {
        texture_coords_arr = malloc(sizeof(float) * vertices.size * 2);
        log_assert(texture_coords_arr != NULL, "Error creating mesh. Out of memory");
    }

    if (use_normals)
    {
        normals_arr = malloc(sizeof(float) * vertices.size * 3);
        log_assert(normals_arr != NULL, "Error creating mesh. Out of memory");
    }

    float *v_itr = vertices_arr;
    float *tc_itr = texture_coords_arr;
    float *n_itr = normals_arr;

    Vertex *itr = (Vertex*)vertices.arr;

    for (uint32_t i = 0; i < vertices.size; ++i)
    {
        Vertex v = *(itr++);

        *(v_itr++) = v.position.x;
        *(v_itr++) = v.position.y;
        *(v_itr++) = v.position.z;

        if (use_texture_coords)
        {
            Vec2 tc = *(Vec2*)vector_get(&texture_coords, v.tex_index);

            *(tc_itr++) = tc.x;
            *(tc_itr++) = tc.y;
        }

        if (use_normals)
        {
            Vec3 n = *(Vec3*)vector_get(&normals, v.norm_index);

            *(n_itr++) = n.x;
            *(n_itr++) = n.y;
            *(n_itr++) = n.z;
        }
    }

    vector_destroy(&vertices);
    vector_destroy(&texture_coords);
    vector_destroy(&normals);
    //vector_destroy(&indices); This vectors pointer is used in mesh

    res.vertices = vertices_arr;
    res.texture_coords = texture_coords_arr;
    res.normals = normals_arr;
    res.num_vertices = vertices.size;
    res.indices = (uint32_t*)indices.arr;
    res.num_indices = indices.size;

    return res;
}