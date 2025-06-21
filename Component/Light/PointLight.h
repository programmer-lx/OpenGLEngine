#pragma once

#include "LightBase.h"

#include <string>
#include <array>

struct ShaderPointLight
{
    glm::vec4 position;
    glm::vec4 color;

    float kc;
    float kl;
    float kq;
    float _pad = 1;
};

class PointLight : public LightBase
{
    FCOMPONENT(PointLight)

public:
    float kc = 1.0f;
    float kl = 0.9f;
    float kq = 0.032f;

private:
    ShaderPointLight m_ShaderData{};

public:
    static std::shared_ptr<PointLight> create(bool _castShadow = false);

public:
    virtual void onInspectorGUI() override;
    virtual void awake() override;
    virtual void tick() override;
    virtual void onDestroy() override;
    virtual void onEnable() override;
    virtual void onDisable() override;

    std::array<glm::mat4x4, 6> cubeViewMatrices() const;

    virtual glm::mat4x4 projectionMatrix() const override;

    virtual void* shaderLightBufferData() override;
};
