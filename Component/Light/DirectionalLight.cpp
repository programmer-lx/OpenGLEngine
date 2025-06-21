#include "DirectionalLight.h"

#include "Core/GameObject.h"
#include "Render/Material.h"
#include "Managers/LightsManager.h"

#include <imgui/imgui.h>

std::shared_ptr<DirectionalLight> DirectionalLight::create(bool castShadow)
{
    auto ptr = std::shared_ptr<DirectionalLight>(new DirectionalLight);
    ptr->castShadow = castShadow;

    return ptr;
}

glm::mat4x4 DirectionalLight::projectionMatrix() const
{
    return glm::ortho(-projectionWidth, projectionWidth, -projectionWidth, projectionWidth, projectionNear, projectionFar);
}

void DirectionalLight::awake()
{
    LightsManager::instance().addDirLight(this);
}

void DirectionalLight::onInspectorGUI()
{
    LightBase::onInspectorGUI();

    ImGui::DragFloat("Projection Width", &projectionWidth, 0.01f, 0, 1000);
}

void DirectionalLight::tick()
{
    m_ShaderData.direction = glm::vec4(transform().forward(), 1);
    m_ShaderData.color = glm::vec4(color, 1);
}

void DirectionalLight::onDestroy()
{
    LightsManager::instance().removeDirLight(this);
}

void DirectionalLight::onEnable()
{
    LightsManager::instance().addDirLight(this);
}

void DirectionalLight::onDisable()
{
    LightsManager::instance().removeDirLight(this);
}

void* DirectionalLight::shaderLightBufferData()
{
    return &m_ShaderData;
}
