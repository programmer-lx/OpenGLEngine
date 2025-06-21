#include "CameraMovement.h"

#include "imgui.h"
#include "Core/Time.h"
#include "Core/GameObject.h"
#include "Core/Subsystems/InputSubsystem.h"
#include "Core/Transform.h"
#include "Misc/Cursor.h"
#include "Render/Material.h"

std::shared_ptr<CameraMovement> CameraMovement::create()
{
    std::shared_ptr<CameraMovement> camera(new CameraMovement);
    return camera;
}

void CameraMovement::tick()
{
    InputSubsystem* input = InputSubsystem::instance();
    Transform& trans = gameObject()->transform();

    if (input->mouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
    {
        canMove = true;
        Cursor::setShowCursor(false);
    }
    if (input->mouseButtonUp(GLFW_MOUSE_BUTTON_RIGHT))
    {
        canMove = false;
        Cursor::setShowCursor(true);
    }

    if (canMove)
    {
        // camera rotate
        glm::vec2 mouseDelta = input->mousePositionDeltaNormalized();
        // 水平旋转：绕世界Y轴（全局旋转）
        trans.worldRotate(-mouseDelta.x * rotateSpeed * Time::unscaledDeltaTime(), Transform::worldUp());
        // 垂直旋转：绕局部X轴（局部旋转）
        trans.localRotate(mouseDelta.y * rotateSpeed * Time::unscaledDeltaTime(), Transform::worldRight());

        // movement
        float forwardDir = input->keyTriggered(GLFW_KEY_W);
        forwardDir = input->keyTriggered(GLFW_KEY_S) ? -1 : forwardDir;
        // Debug::print("fwd dir: {}", forwardDir);

        float rightDir = input->keyTriggered(GLFW_KEY_D);
        rightDir = input->keyTriggered(GLFW_KEY_A) ? -1 : rightDir;

        float upDir = input->keyTriggered(GLFW_KEY_E);
        upDir = input->keyTriggered(GLFW_KEY_Q) ? -1 : upDir;

        moveSpeed += input->mouseScrollDelta().y * 3;
        moveSpeed = glm::max(0.0f, moveSpeed);

        if (forwardDir != 0 || rightDir != 0 || upDir != 0)
        {
            glm::vec3 dir = glm::normalize(glm::vec3(rightDir, upDir, -forwardDir));
            trans.localTranslate(dir * Time::unscaledDeltaTime() * moveSpeed);
        }
    }
}

void CameraMovement::onInspectorGUI()
{
    ComponentBase::onInspectorGUI();

    ImGui::DragFloat("Move Speed", &moveSpeed, 0.05f, 0);
    ImGui::DragFloat("Rotate Speed", &rotateSpeed, 0.05f, 5000, 20000);
}
