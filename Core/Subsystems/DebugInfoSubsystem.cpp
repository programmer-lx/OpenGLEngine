#include "DebugInfoSubsystem.h"

#include "Core/Time.h"
#include "Misc/Timer.h"

#include <GLFW/glfw3.h>

DebugInfoSubsystem::DebugInfoSubsystem(const std::string& defaultTitle, GLFWwindow* window)
{
    m_DefaultTitle = defaultTitle;
    m_Window = window;

    m_FramerateTimer = std::make_unique<Timer>(0.5f, [&]()
    {
        std::string fpsStr = std::to_string(1.0f / Time::deltaTime());
        std::string timeStr = std::to_string(Time::unscaledTime());
        glfwSetWindowTitle(m_Window, (m_DefaultTitle + ", FPS: " + fpsStr + ", Unscaled Time: " + timeStr).c_str());
    });
}

void DebugInfoSubsystem::tick()
{
    m_FramerateTimer->tick();
}
