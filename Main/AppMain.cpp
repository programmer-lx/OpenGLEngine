#include "AppMain.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core/Application.h"
#include "Core/Subsystems/InputSubsystem.h"
#include "Core/Subsystems/PostInputSubsystem.h"
#include "Core/Subsystems/TimeSubsystem.h"
#include "Core/Subsystems/DebugInfoSubsystem.h"
#include "Core/Subsystems/SceneSubsystem.h"
#include "Core/Subsystems/PlayerLoopSubsystems/TickSubsystem.h"
#include "Core/Subsystems/PlayerLoopSubsystems/FixedTickSubsystem.h"
#include "Core/Subsystems/RenderPipelineSubsystem.h"
#include "Core/Subsystems/EditorUI/EditorUISubsystem.h"

#include "Managers/SubsystemManager.h"

#include "Graphic/Graphics.h"

#include "Misc/Screen.h"
#include "Misc/Cursor.h"

#include "Platform/WindowsPlatform.h"

#include "Game/Scenes/FirstScene.h"

#include <iostream>

// constant variables
constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;

// callback when window resize
void windowResizeCallback(GLFWwindow* window, int width, int height)
{
    Screen::notifyResize(width, height);
}

// callback when mouse scroll value changed
void mouseScrollCallback(GLFWwindow* window, double x, double y)
{
    InputSubsystem::instance()->notifyScrollValueChanged(x, y);
}

// callback when mouse move
bool g_IsFirstMouse = true;
void mouseMoveCallback(GLFWwindow* window, double x, double y)
{
    if (g_IsFirstMouse)
    {
        InputSubsystem::instance()->setMouseOriginalPosition(x, y);
        g_IsFirstMouse = false;
        return;
    }
    InputSubsystem::instance()->notifyMousePositionChanged(x, y);
}

int AppMain::run(const std::vector<std::string>& cmdLine)
{
    // initialize glfw
    glfwInit();

    // version check
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Debug Context
#if FLX_DEBUG_BUILD
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

    // create a window
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "FlxEngine", nullptr, nullptr);
    Application::init(window);
    Cursor::init(window);
    if (window == nullptr)
    {
        // create failed
        std::cout << "Fail to create GLFW window." << std::endl;

        // clean up
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // load all opengl function pointers
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        // load failed
        std::cout << "Fail to load opengl function pointers." << std::endl;

        glfwTerminate();
        return -1;
    }

    // ========================================== register GLFW callbacks ==========================================
    glfwSetFramebufferSizeCallback(window, windowResizeCallback);
    glfwSetScrollCallback(window, mouseScrollCallback);
    glfwSetCursorPosCallback(window, mouseMoveCallback);
    // ========================================== end register GLFW callbacks ==========================================

    // platform settings
    WindowsPlatform::instance().init();

    // Graphics init
    Graphics::init();

    // init screen
    Screen::notifyResize(WINDOW_WIDTH, WINDOW_HEIGHT);

    // ========================================== register subsystems ==========================================
    SubsystemManager& subsystemManager = SubsystemManager::instance();

    // input subsystem
    subsystemManager.add<InputSubsystem>(window);

    // ======================================= PlayerLoop Subsystems =======================================
    // scene Tick
    subsystemManager.add<SceneSubsystem>();
    SceneSubsystem::instance()->loadScene<FirstScene>();

    // FixedTick
    subsystemManager.add<FixedTickSubsystem>();

    // Tick
    subsystemManager.add<TickSubsystem>();
    // ======================================= End PlayerLoop Subsystems =======================================

    // post input subsystem
    subsystemManager.add<PostInputSubsystem>();

    // game render pipeline subsystem
    subsystemManager.add<RenderPipelineSubsystem>();

    // editor ui render subsystem
    subsystemManager.add<EditorUISubsystem>(window);

    // debug info subsystem
    // #if FLX_DEBUG_BUILD
    subsystemManager.add<DebugInfoSubsystem>("FlxEngine", window);
    // #endif

    // timer subsystem (must at the end)
    subsystemManager.add<TimeSubsystem>();
    // ========================================== end register subsystems ==========================================

    /////////////////////////////////////////////////////////////////////////////////////////////
    // loop
    glfwSwapInterval(0);
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // subsystem tick
        subsystemManager.tick();

        // swap buffers
        glfwSwapBuffers(window);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////

    // clean all subsystems
    subsystemManager.shutdown();

    Graphics::shutdown();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
