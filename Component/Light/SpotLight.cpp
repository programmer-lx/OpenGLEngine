#include "SpotLight.h"

#include "Core/GameObject.h"
#include "Render/Material.h"
#include "Managers/LightsManager.h"

#include <imgui/imgui.h>

std::shared_ptr<SpotLight> SpotLight::create()
{
    auto ptr = std::shared_ptr<SpotLight>(new SpotLight);
    return ptr;
}

void SpotLight::awake()
{
    LightsManager::instance().addSpotLight(this);
}

void SpotLight::tick()
{
    m_ShaderData.position = glm::vec4(transform().position(), 1);
    m_ShaderData.direction = glm::vec4(transform().forward(), 1);
    m_ShaderData.color = glm::vec4(color, 1);
    m_ShaderData.cosInnerCone = cosInnerCone;
    m_ShaderData.cosOuterCone = cosOuterCone;
    m_ShaderData.kc = kc;
    m_ShaderData.kl = kl;
    m_ShaderData.kq = kq;
}

void SpotLight::onInspectorGUI()
{
    LightBase::onInspectorGUI();

    ImGui::DragFloat("Kc", &kc, 0.01f, 1, 10);
    ImGui::DragFloat("Kl", &kl, 0.01f, 0, 10);
    ImGui::DragFloat("Kq", &kq, 0.01f, 0, 10);
    ImGui::DragFloat("cos inner cone", &cosInnerCone, 0.01f, 0, 1);
    ImGui::DragFloat("cos outer cone", &cosOuterCone, 0.01f, 0, 1);
    cosOuterCone = std::min(cosOuterCone, cosInnerCone);
}

void SpotLight::onDestroy()
{
    LightsManager::instance().removeSpotLight(this);
}

void SpotLight::onEnable()
{
    LightsManager::instance().addSpotLight(this);
}

void SpotLight::onDisable()
{
    LightsManager::instance().removeSpotLight(this);
}

void* SpotLight::shaderLightBufferData()
{
    return &m_ShaderData;
}
