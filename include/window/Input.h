#pragma once

#include"Core.h"

typedef int IC_KEY;
typedef int IC_MOUSE_BUTTON;

#define IC_KEY_A      0
#define IC_KEY_B      1
#define IC_KEY_C      2
#define IC_KEY_D      3
#define IC_KEY_E      4
#define IC_KEY_F      5 
#define IC_KEY_G      6
#define IC_KEY_H      7
#define IC_KEY_I      8
#define IC_KEY_J      9
#define IC_KEY_K      10
#define IC_KEY_L      11
#define IC_KEY_M      12
#define IC_KEY_N      13
#define IC_KEY_O      14
#define IC_KEY_P      15
#define IC_KEY_Q      16
#define IC_KEY_R      17
#define IC_KEY_S      18
#define IC_KEY_T      19
#define IC_KEY_U      20
#define IC_KEY_V      21
#define IC_KEY_W      22
#define IC_KEY_X      23
#define IC_KEY_Y      24
#define IC_KEY_Z      25
#define IC_KEY_0      26
#define IC_KEY_1      27
#define IC_KEY_2      28
#define IC_KEY_3      29
#define IC_KEY_4      30
#define IC_KEY_5      31
#define IC_KEY_6      32
#define IC_KEY_7      33
#define IC_KEY_8      34
#define IC_KEY_9      35
#define IC_KEY_UP     36
#define IC_KEY_DOWN   37
#define IC_KEY_LEFT   38
#define IC_KEY_RIGHT  39
#define IC_KEY_SPACE  40
#define IC_KEY_SHIFT  41
#define IC_KEY_ESCAPE 42
#define IC_KEY_ENTER  43
#define IC_KEY_LAST   44

#define IC_MOUSE_BUTTON_LEFT   0
#define IC_MOUSE_BUTTON_RIGHT  1
#define IC_MOUSE_BUTTON_MIDDLE 2
#define IC_MOUSE_BUTTON_LAST   3

typedef struct _Window Window;

struct _Input
{
    const Window *window;
    
    IC_BOOL _keys[IC_KEY_LAST];
    IC_BOOL _mouse_buttons[IC_MOUSE_BUTTON_LAST];
    float _cx_o, _cy_o, _cx_n, _cy_n;
};

typedef struct _Input Input;

IC_ERROR_CODE input_create(Input *input);
void input_destroy(const Input *input);
void input_update(Input *input);
IC_BOOL input_key_down(const Input *input, IC_KEY key);
IC_BOOL input_key_pressed(const Input *input, IC_KEY key);
IC_BOOL input_key_released(const Input *input, IC_KEY key);
IC_BOOL input_mouse_button_down(const Input *input, IC_MOUSE_BUTTON mouse_button);
IC_BOOL input_mouse_button_pressed(const Input *input, IC_MOUSE_BUTTON mouse_button);
IC_BOOL input_mouse_button_released(const Input *input, IC_MOUSE_BUTTON mouse_button);
float input_cursor_x(const Input *input);
float input_cursor_y(const Input *input);
float input_cursor_dx(const Input *input);
float input_cursor_dy(const Input *input);
void input_disable_cursor(const Input *input);
void input_enable_cursor(const Input *input);
IC_BOOL input_cursor_enabled(const Input *input);
void input_toggle_cursor(const Input *input);