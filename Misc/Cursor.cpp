#include "Cursor.h"

#include "Core/Subsystems/InputSubsystem.h"

#include <GLFW/glfw3.h>

GLFWwindow* Cursor::s_Window = nullptr;

void Cursor::init(GLFWwindow* window)
{
    s_Window = window;
}

void Cursor::setShowCursor(bool show)
{
    int mode{};
    if (show)
        mode = GLFW_CURSOR_NORMAL;
    else
        mode = GLFW_CURSOR_DISABLED;

    glfwSetInputMode(s_Window, GLFW_CURSOR, mode);

    double x, y;
    glfwGetCursorPos(s_Window, &x, &y);
    InputSubsystem::instance()->setMouseOriginalPosition(x, y);
}
