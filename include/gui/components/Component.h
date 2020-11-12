#pragma once

#include"util/math/Vec.h"
#include"util/Color.h"
#include"Core.h"

struct _Gui_Pane
{
    Vec2 center;
    Vec2 extents;
    IC_BOOL fixed;

    Color color;
};

struct _Gui_Panel
{
    Color color;
};

typedef struct _Gui_Pane Gui_Pane;
typedef struct _Gui_Panel Gui_Panel;

typedef struct _Gui_Shader Gui_Shader;
typedef struct _Window Window;

void gui_pane_render(const Gui_Pane *pane, const Gui_Shader *shader, const Window *window);
void gui_panel_render(const Gui_Panel *panel, const Gui_Shader *shader);