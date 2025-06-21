#include "EditorUISubsystem.h"

#include "Core/GameObject.h"
#include "Core/Subsystems/SceneSubsystem.h"
#include "Core/Scene.h"
#include "Render/ShaderUtil.h"
#include "Misc/Debug.h"
#include "Misc/Screen.h"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void EditorUISubsystem::drawInspectorGUI(SceneSubsystem* sceneSubsystem)
{
    // 获取主视口信息
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 viewportSize = io.DisplaySize;

    // 计算侧边栏的位置和大小
    ImVec2 sidebarSize = ImVec2(viewportSize.x * 0.25f, viewportSize.y); // 宽度为屏幕的1/4，高度为全屏
    ImVec2 sidebarPos = ImVec2(viewportSize.x - sidebarSize.x, 0); // 位置固定在右侧

    // 设置下一个窗口的位置和大小
    ImGui::SetNextWindowPos(sidebarPos);
    ImGui::SetNextWindowSize(sidebarSize);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;

    // create Inspector Panel
    ImGui::Begin("Inspector", nullptr, window_flags);

    if (GameObject* o = sceneSubsystem->selectedGameObject())
    {
        o->drawInspectorGUI();
    }
    else
    {
        ImGui::Text("No Selected GameObject");
    }

    ImGui::End();
}

EditorUISubsystem::EditorUISubsystem(GLFWwindow* window)
{
    Debug::print("Init ImGui");

    // init imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(ShaderUtil::GLSL_VERSION_STR);
}

EditorUISubsystem::~EditorUISubsystem()
{
    Debug::print("Destroy ImGui");

    // clean up imgui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void EditorUISubsystem::tick()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, Screen::width(), Screen::height());

    // start imgui new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (SceneSubsystem* sceneSubsystem = SceneSubsystem::instance())
    {
        // Scene onEditorGUI() callback
        sceneSubsystem->curScene()->onEditorGUI();

        // GameObject onInspectorGUI() callback
        drawInspectorGUI(sceneSubsystem);
    }

    // render ui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
