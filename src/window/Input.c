#include"window/Input.h"
#include"window/Window.h"
#include"window/Window_Mmt.h"

#define GLFW_INCLUDE_NONE
#include<GLFW/glfw3.h>

#include<string.h>

const int IC_TO_GLFW_KEY[IC_KEY_LAST] =
{
    GLFW_KEY_A,
    GLFW_KEY_B,
    GLFW_KEY_C,
    GLFW_KEY_D,
    GLFW_KEY_E,
    GLFW_KEY_F,
    GLFW_KEY_G,
    GLFW_KEY_H,
    GLFW_KEY_I,
    GLFW_KEY_J,
    GLFW_KEY_K,
    GLFW_KEY_L,
    GLFW_KEY_M,
    GLFW_KEY_N,
    GLFW_KEY_O,
    GLFW_KEY_P,
    GLFW_KEY_Q,
    GLFW_KEY_R,
    GLFW_KEY_S,
    GLFW_KEY_T,
    GLFW_KEY_U,
    GLFW_KEY_V,
    GLFW_KEY_W,
    GLFW_KEY_X,
    GLFW_KEY_Y,
    GLFW_KEY_Z,
    GLFW_KEY_0,
    GLFW_KEY_1,
    GLFW_KEY_2,
    GLFW_KEY_3,
    GLFW_KEY_4,
    GLFW_KEY_5,
    GLFW_KEY_6,
    GLFW_KEY_7,
    GLFW_KEY_8,
    GLFW_KEY_9,
    GLFW_KEY_UP,
    GLFW_KEY_DOWN,
    GLFW_KEY_LEFT,
    GLFW_KEY_RIGHT,
    GLFW_KEY_SPACE,
    -1
};

const int IC_TO_GLFW_MOUSE_BUTTON[IC_MOUSE_BUTTON_LAST] =
{
    GLFW_MOUSE_BUTTON_LEFT,
    GLFW_MOUSE_BUTTON_RIGHT,
    GLFW_MOUSE_BUTTON_MIDDLE
};

void input_create(Input *input)
{
    memset(input->keys, 0, sizeof(input->keys));
    memset(input->mouse_buttons, 0, sizeof(input->mouse_buttons));
}

void input_update(Input *input)
{
    for (IC_KEY key = 0; key < IC_KEY_LAST; ++key) input->keys[key] = input_key_down(input, key);
    for (IC_MOUSE_BUTTON mouse_button = 0; mouse_button < IC_MOUSE_BUTTON_LAST; ++mouse_button) input->mouse_buttons[mouse_button] = input_mouse_button_down(input, mouse_button);
}

IC_BOOL input_key_down(const Input *input, IC_KEY key)
{
    switch (key)
    {
    case IC_KEY_SHIFT:
        return glfwGetKey(input->window->data->handle, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS
            || glfwGetKey(input->window->data->handle, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
    
    default:
        return glfwGetKey(input->window->data->handle, IC_TO_GLFW_KEY[key]) == GLFW_PRESS;
    }
}

IC_BOOL input_key_pressed(const Input *input, IC_KEY key)
{
    return input_key_down(input, key) && !input->keys[key];
}

IC_BOOL input_key_released(const Input *input, IC_KEY key)
{
    return !input_key_down(input, key) && input->keys[key];
}

IC_BOOL input_mouse_button_down(const Input *input, IC_MOUSE_BUTTON mouse_button)
{
    return glfwGetMouseButton(input->window->data->handle, IC_TO_GLFW_MOUSE_BUTTON[mouse_button]) == GLFW_PRESS;
}

IC_BOOL input_mouse_button_pressed(const Input *input, IC_MOUSE_BUTTON mouse_button)
{
    return input_mouse_button_down(input, mouse_button) && !input->mouse_buttons[mouse_button];
}

IC_BOOL input_mouse_button_released(const Input *input, IC_MOUSE_BUTTON mouse_button)
{
    return !input_mouse_button_down(input, mouse_button) && input->mouse_buttons[mouse_button];
}