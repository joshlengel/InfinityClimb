#include"window/Window.h"
#include"window/Window_Mmt.h"
#include"Log.h"

#define GLFW_INCLUDE_NONE
#include<GLFW/glfw3.h>
#include<glad/glad.h>

#include<stdlib.h>

void window_resize_callback(GLFWwindow *glfw_window, int width, int height)
{
    glViewport(0, 0, width, height);

    Window *window = (Window*)glfwGetWindowUserPointer(glfw_window);
    window->width = (uint32_t)width;
    window->height = (uint32_t)height;
}

IC_ERROR_CODE window_create(Window *dest)
{
    Window_Data *data = malloc(sizeof(Window_Data));
    dest->data = data;
    log_assert(data != NULL, "Error creating window. Out of memory");

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    data->handle = glfwCreateWindow((int)dest->width, (int)dest->height, dest->title, NULL, NULL);

    if (!data->handle)
    {
        free(data);
        return IC_WINDOW_CREATE_ERROR;
    }

    glfwSetWindowUserPointer(data->handle, dest);

    // Center window in screen
    const GLFWvidmode *primary_monitor = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwSetWindowPos(data->handle, (primary_monitor->width - dest->width) / 2, (primary_monitor->height - dest->height) / 2);

    glfwMakeContextCurrent(data->handle);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        window_destroy(dest);
        return IC_WINDOW_GL_CONTEXT_ERROR;
    }

    glfwSetWindowSizeCallback(data->handle, window_resize_callback);

    return IC_NO_ERROR;
}

void window_destroy(const Window *window)
{
    glfwDestroyWindow(window->data->handle);

    free((void*)window->data);
}

float window_aspect_ratio(const Window *window)
{
    int width[1];
    int height[1];
    glfwGetWindowSize(window->data->handle, width, height);

    return (float) width[0] / (float) height[0];
}

void window_show(const Window *window)
{
    glfwShowWindow(window->data->handle);
}

IC_BOOL window_should_close(const Window *window)
{
    return glfwWindowShouldClose(window->data->handle);
}

void window_swap_buffers(const Window *window)
{
    glfwSwapBuffers(window->data->handle);
}

void window_poll_events()
{
    glfwPollEvents();
}