#pragma once

#include "LightBase.h"

struct ShaderDirLight
{
    glm::vec4 direction;
    glm::vec4 color;
};

class DirectionalLight : public LightBase
{
    FCOMPONENT(DirectionalLight)

private:
    ShaderDirLight m_ShaderData{};

public:
    float projectionWidth = 500;

public:
    static std::shared_ptr<DirectionalLight> create(bool castShadow = false);

    virtual glm::mat4x4 projectionMatrix() const override;

    virtual void awake() override;
    virtual void onInspectorGUI() override;
    virtual void tick() override;
    virtual void onDestroy() override;
    virtual void onEnable() override;
    virtual void onDisable() override;

    virtual void* shaderLightBufferData() override;
};
