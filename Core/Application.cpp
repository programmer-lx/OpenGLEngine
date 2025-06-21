#include "Application.h"

#include "Config/Config.h"
#include "Platform/WindowsPlatform.h"

#include <GLFW/glfw3.h>

GLFWwindow* Application::s_Window = nullptr;
std::string Application::s_DataPath;

void Application::init(GLFWwindow* window)
{
    s_Window = window;
}

const std::string& Application::dataPath()
{
    if (s_DataPath.empty())
    {
#if FLX_DEBUG_BUILD
        s_DataPath = FLX_DEBUG_DATA_PATH;
#else
        s_DataPath = WindowsPlatform::instance().exeDir();
#endif
    }
    return s_DataPath;
}

std::string Application::shadersPath()
{
    return dataPath() + "/Shaders";
}

std::string Application::resourcesPath()
{
    return dataPath() + "/Resources";
}

void Application::quit()
{
    glfwSetWindowShouldClose(s_Window, true);
}
