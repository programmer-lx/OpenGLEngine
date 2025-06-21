#pragma once

#include "LightBase.h"

struct ShaderSpotLight
{
    glm::vec4 position;
    glm::vec4 direction;

    glm::vec4 color;

    float cosInnerCone;
    float cosOuterCone;

    float kc;
    float kl;
    float kq;

    glm::vec3 _pad = {1, 1, 1};
};

class SpotLight : public LightBase
{
    FCOMPONENT(SpotLight)

private:
    ShaderSpotLight m_ShaderData{};

public:
    float kc = 1.0f;
    float kl = 0.9f;
    float kq = 0.032f;
    float cosInnerCone = 0.7f;
    float cosOuterCone = 0.5f;

public:
    static std::shared_ptr<SpotLight> create();

public:
    virtual void awake() override;
    virtual void tick() override;
    virtual void onInspectorGUI() override;
    virtual void onDestroy() override;
    virtual void onEnable() override;
    virtual void onDisable() override;

    virtual glm::mat4x4 projectionMatrix() const override { return {}; }

    virtual void* shaderLightBufferData() override;
};
