#include "LightsManager.h"

void LightsManager::resetLightCount()
{
    m_UBO_LightCount->bind();
    m_UBO_LightCount->startAddSubData_BindBufferBase(2);

    std::int32_t pointCount = m_PointLights.size();
    std::int32_t spotCount = m_SpotLights.size();
    std::int32_t dirCount = m_DirLights.size();
    m_UBO_LightCount->addSubData<float>(&pointCount);
    m_UBO_LightCount->addSubData<float>(&spotCount);
    m_UBO_LightCount->addSubData<float>(&dirCount);
    m_UBO_LightCount->addSubData<float>(&m_ShadowPointLightIndex);
    m_UBO_LightCount->addSubData<float>(&m_ShadowDirLightIndex);

    m_UBO_LightCount->endAddSubData();
    m_UBO_LightCount->unbind();
}

LightsManager::LightsManager()
{
    m_SSBO_PointLights = std::make_unique<BufferObject>(GL_SHADER_STORAGE_BUFFER,
                                                        sizeof(ShaderPointLight) * POINT_LIGHT_MAX_COUNT,
                                                        nullptr,
                                                        GL_DYNAMIC_DRAW);

    m_SSBO_DirLights = std::make_unique<BufferObject>(GL_SHADER_STORAGE_BUFFER,
                                                      sizeof(ShaderDirLight) * DIR_LIGHT_MAX_COUNT,
                                                      nullptr,
                                                      GL_DYNAMIC_DRAW);

    m_SSBO_SpotLights = std::make_unique<BufferObject>(GL_SHADER_STORAGE_BUFFER,
                                                       sizeof(ShaderSpotLight) * SPOT_LIGHT_MAX_COUNT,
                                                       nullptr,
                                                       GL_DYNAMIC_DRAW);

    m_UBO_LightCount = std::make_unique<BufferObject>(GL_UNIFORM_BUFFER, 32 * sizeof(float), nullptr, GL_STATIC_DRAW);
}

LightsManager::~LightsManager()
{
}

void LightsManager::addDirLight(DirectionalLight* light)
{
    m_DirLights.emplace_back(light);

    if (light->castShadow && m_ShadowMapDirLight == nullptr)
    {
        m_ShadowMapDirLight = light;
        m_ShadowDirLightIndex = m_DirLights.size() - 1;
    }

    resetDirLightBuffer();
    resetLightCount();
}

void LightsManager::removeDirLight(DirectionalLight* light)
{
    erase_if(m_DirLights, [&](DirectionalLight* l)
    {
        return l->componentID() == light->componentID();
    });

    resetDirLightBuffer();
    resetLightCount();
}

void LightsManager::resetDirLightBuffer()
{
    m_SSBO_DirLights->bind();
    m_SSBO_DirLights->startAddSubData_BindBufferBase(2);

    for (auto& light : m_DirLights)
    {
        m_SSBO_DirLights->addSubData<ShaderDirLight>(light->shaderLightBufferData());
    }

    m_SSBO_DirLights->endAddSubData();
}

void LightsManager::addPointLight(PointLight* light)
{
    m_PointLights.emplace_back(light);

    if (light->castShadow && m_ShadowMapPointLight == nullptr)
    {
        m_ShadowMapPointLight = light;
        m_ShadowPointLightIndex = m_PointLights.size() - 1;
    }

    resetPointLightBuffer();
    resetLightCount();
}

void LightsManager::removePointLight(PointLight* light)
{
    erase_if(m_PointLights, [&](PointLight* l)
    {
        return l->componentID() == light->componentID();
    });

    resetPointLightBuffer();
    resetLightCount();
}

void LightsManager::resetPointLightBuffer()
{
    m_SSBO_PointLights->bind();
    m_SSBO_PointLights->startAddSubData_BindBufferBase(0);

    for (auto& pointLight : m_PointLights)
    {
        m_SSBO_PointLights->addSubData<ShaderPointLight>(pointLight->shaderLightBufferData());
    }

    m_SSBO_PointLights->endAddSubData();
}

void LightsManager::addSpotLight(SpotLight* light)
{
    m_SpotLights.emplace_back(light);

    resetSpotLightBuffer();
    resetLightCount();
}

void LightsManager::removeSpotLight(SpotLight* light)
{
    erase_if(m_SpotLights, [&](SpotLight* l)
    {
        return l->componentID() == light->componentID();
    });

    resetSpotLightBuffer();
    resetLightCount();
}

void LightsManager::resetSpotLightBuffer()
{
    m_SSBO_SpotLights->bind();
    m_SSBO_SpotLights->startAddSubData_BindBufferBase(1);

    for (auto& light : m_SpotLights)
    {
        m_SSBO_SpotLights->addSubData<ShaderSpotLight>(light->shaderLightBufferData());
    }

    m_SSBO_SpotLights->endAddSubData();
}
