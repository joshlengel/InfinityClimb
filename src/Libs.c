#include"Log_Mmt.h"
#include"gui/Gui_Lib.h"

#include"Core.h"

#define GLFW_INCLUDE_NONE
#include<GLFW/glfw3.h>

IC_ERROR_CODE load_libs()
{
    if (!glfwInit())
    {
        return IC_WINDOW_LIB_LOAD_ERROR;
    }

    if (log_init() != IC_NO_ERROR)
    {
        glfwTerminate();
        return IC_LOG_LIB_LOAD_ERROR;
    }

    return IC_NO_ERROR;
}

void terminate_libs()
{
    gui_terminate_lib();
    
    log_terminate();

    glfwTerminate();
}