#include "PointLight.h"

#include "Render/Material.h"
#include "Core/GameObject.h"
#include "Managers/LightsManager.h"

#include <imgui/imgui.h>

std::shared_ptr<PointLight> PointLight::create(bool _castShadow)
{
    auto ptr = std::shared_ptr<PointLight>(new PointLight);
    ptr->castShadow = _castShadow;
    return ptr;
}

void PointLight::onInspectorGUI()
{
    LightBase::onInspectorGUI();

    ImGui::DragFloat("Kc", &kc, 0.01f, 1, 10);
    ImGui::DragFloat("Kl", &kl, 0.01f, 0, 10);
    ImGui::DragFloat("Kq", &kq, 0.01f, 0, 10);
}

void PointLight::awake()
{
    LightsManager::instance().addPointLight(this);
}

void PointLight::tick()
{
    m_ShaderData.position = glm::vec4(transform().position(), 1);
    m_ShaderData.color = glm::vec4(color, 1);
    m_ShaderData.kc = kc;
    m_ShaderData.kl = kl;
    m_ShaderData.kq = kq;
}

void PointLight::onDestroy()
{
    LightsManager::instance().removePointLight(this);
}

void PointLight::onEnable()
{
    LightsManager::instance().addPointLight(this);
}

void PointLight::onDisable()
{
    LightsManager::instance().removePointLight(this);
}

std::array<glm::mat4x4, 6> PointLight::cubeViewMatrices() const
{
    // clang-format off
    return {
        glm::lookAt(transform().position(), transform().position() + glm::vec3(1, 0, 0), glm::vec3(0, -1, 0)),  // right
        glm::lookAt(transform().position(), transform().position() + glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0)), // left
        glm::lookAt(transform().position(), transform().position() + glm::vec3(0, 1, 0), glm::vec3(0, 0, 1)),   // up
        glm::lookAt(transform().position(), transform().position() + glm::vec3(0, -1, 0), glm::vec3(0, 0, -1)), // down
        glm::lookAt(transform().position(), transform().position() + glm::vec3(0, 0, 1), glm::vec3(0, -1, 0)),  // near
        glm::lookAt(transform().position(), transform().position() + glm::vec3(0, 0, -1), glm::vec3(0, -1, 0)), // far
    };
    // clang-format on
}

glm::mat4x4 PointLight::projectionMatrix() const
{
    return glm::perspective(glm::radians(90.0f), 1.0f, projectionNear, projectionFar);
}

void* PointLight::shaderLightBufferData()
{
    return &m_ShaderData;
}
