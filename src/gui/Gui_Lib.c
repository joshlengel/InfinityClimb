#include<glad/glad.h>
#include<stddef.h>

GLuint gui_vao_id;

static GLuint gui_buff_id;

void gui_init_lib()
{
    glGenVertexArrays(1, &gui_vao_id);
    glGenBuffers(1, &gui_buff_id);

    glBindVertexArray(gui_vao_id);

    glBindBuffer(GL_ARRAY_BUFFER, gui_buff_id);
    
    float vertices[] =
    {
        -1.0f, -1.0f,
         1.0f, -1.0f,
         1.0f,  1.0f,
        -1.0f, -1.0f,
         1.0f,  1.0f,
        -1.0f,  1.0f
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);
}

void gui_terminate_lib()
{
    glDeleteBuffers(1, &gui_buff_id);
    glDeleteVertexArrays(1, &gui_vao_id);
}