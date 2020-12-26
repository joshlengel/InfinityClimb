#include"state/Game_State.h"
#include"state/State.h"

#include"gui/components/Component.h"
#include"gui/text/Text_Shader.h"
#include"gui/text/Text.h"
#include"gui/text/Font.h"
#include"gui/Gui_Shader.h"
#include"gui/Gui_Lib.h"
#include"gui/Gui_Utils.h"
#include"window/Window.h"
#include"window/Input.h"
#include"window/Context.h"
#include"util/Utils.h"
#include"Core.h"
#include"Log.h"

#include<stdlib.h>

struct _Start_State_Data
{
    IC_BOOL init_failed;

    Context context;

    Loader loader;

    Gui_Panel gui_backdrop;
    Gui_Pane gui_content;
    Gui_Shader gui_shader;

    Font gui_font;
    Text gui_text;
    Text_Shader gui_text_shader;
};

typedef struct _Start_State_Data Start_State_Data;

void start_state_start(State *state)
{
    Start_State_Data *data = malloc(sizeof(Start_State_Data));
    log_assert(data != NULL, "Error starting start state. Out of memory");
    state->data = data;

    data->init_failed = IC_FALSE;

    // Context
    data->context.background_color = NULL;
    data->context.clear_color = IC_TRUE;
    data->context.clear_depth = IC_FALSE;
    data->context.cull = IC_TRUE;
    data->context.cull_front = IC_FALSE;
    data->context.depth_test = IC_FALSE;
    data->context.blending = IC_TRUE;

    data->gui_backdrop.color = color_create_hex(0x666666FF);
    data->gui_content.color = color_create_hex(0x6677DDFF);
    data->gui_content.center.x = 0.0f;
    data->gui_content.center.y = 0.0f;
    data->gui_content.fixed = IC_FALSE;

    IC_ERROR_CODE ec;
    data->gui_font = font_load("Arial", &ec);

    data->gui_text.font = &data->gui_font;
    data->gui_text.line_width = 2.0f;
    data->gui_text.size = 1.0f;
    string_create(&data->gui_text.text, "Hello world!");

    if (ec != IC_NO_ERROR)
    {
        state->exit = IC_TRUE;
        data->init_failed = IC_TRUE;
        return;
    }

    gui_init_lib();

    data->loader.num_resources = 5;
    loader_create(&data->loader);

    loader_add_resource(&data->loader, &data->context,         (Loader_Init_proc)context_create,     (Loader_Dest_proc)context_destroy);
    loader_add_resource(&data->loader, &data->gui_shader,      (Loader_Init_proc)gui_shader_create,  (Loader_Dest_proc)gui_shader_destroy);
    loader_add_resource(&data->loader, &data->gui_font,        (Loader_Init_proc)font_create,        (Loader_Dest_proc)font_destroy);
    loader_add_resource(&data->loader, &data->gui_text,        (Loader_Init_proc)text_create,        (Loader_Dest_proc)text_destroy);
    loader_add_resource(&data->loader, &data->gui_text_shader, (Loader_Init_proc)text_shader_create, (Loader_Dest_proc)text_shader_destroy);
    loader_load(&data->loader);

    if (loader_error(&data->loader) != IC_NO_ERROR)
    {
        state->exit = IC_TRUE;
        data->init_failed = IC_TRUE;
        return;
    }

    window_show(state->window);
    timer_start(state->timer);
}

void start_state_stop(State *state)
{
    Start_State_Data *data = (Start_State_Data*)state->data;
    
    // Free resources
    loader_unload(&data->loader);
    loader_destroy(&data->loader);

    string_destroy(&data->gui_text.text);

    if (data->init_failed)
    {
        state->next_state = NULL;
    }
    else
    {
        state->next_state = malloc(sizeof(State));
        state->next_state->exit = IC_FALSE;
        state->next_state->start_proc  = game_state_start;
        state->next_state->stop_proc   = game_state_stop;
        state->next_state->update_proc = game_state_update;
        state->next_state->render_proc = game_state_render;
    }

    free(state->data);
}

void start_state_update(State *state)
{
    if (input_key_pressed(state->input, IC_KEY_SPACE))
    {
        state->exit = IC_TRUE;
    }

    Start_State_Data *data = (Start_State_Data*)state->data;

    data->gui_content.extents.x = gui_width_to_extent(state->window->width - 30 * 2, state->window); // 30 pixels from either edge
    data->gui_content.extents.y = gui_height_to_extent(state->window->height - 30 * 2, state->window); // 30 pixels from either edge
}

void start_state_render(State *state)
{
    Start_State_Data *data = (Start_State_Data*)state->data;
    gui_panel_render(&data->gui_backdrop, &data->gui_shader);
    gui_pane_render(&data->gui_content, &data->gui_shader, state->window);

    Color text_color = color_create_hex(0xFF0000FF); // red
    text_shader_bind(&data->gui_text_shader);
    text_shader_set_aspect_ratio(&data->gui_text_shader, window_aspect_ratio(state->window));
    text_shader_set_color(&data->gui_text_shader, &text_color);
    text_render(&data->gui_text);
}