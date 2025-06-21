#include "LightBase.h"

#include "Core/Transform.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>

void LightBase::onInspectorGUI()
{
    ComponentBase::onInspectorGUI();

    ImGui::ColorEdit3("color", glm::value_ptr(color));

    ImGui::DragFloat("Projection Near", &projectionNear, 0.01f, 0, 1000);
    ImGui::DragFloat("Projection Far", &projectionFar, 0.01f, 0, 1000);
}

glm::mat4x4 LightBase::viewMatrix() const
{
    return glm::lookAt(transform().position(), transform().position() + transform().forward(), Transform::worldUp());
}
