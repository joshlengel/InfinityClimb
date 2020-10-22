#include"Log_Mmt.h"

#include"Core.h"

#define GLFW_INCLUDE_NONE
#include<GLFW/glfw3.h>

IC_ERROR_CODE load_libs()
{
    if (!glfwInit())
    {
        return IC_WINDOW_LIB_LOAD_ERROR;
    }

    log_init();

    return IC_NO_ERROR;
}

void terminate_libs()
{
    log_terminate();

    glfwTerminate();
}