#pragma once

#include "Component/ComponentBase.h"

#include <glm/glm.hpp>

#include <vector>

class Material;

class LightBase : public ComponentBase
{
public:
    glm::vec3 color = {0.5f, 0.5f, 0.5f};

    bool castShadow = false;
    float projectionNear = 0.1f;
    float projectionFar = 200.0f;

public:
    virtual ~LightBase() override = default;
    virtual void onInspectorGUI() override;

    glm::mat4x4 viewMatrix() const;
    virtual glm::mat4x4 projectionMatrix() const = 0;

    virtual void* shaderLightBufferData() = 0;
};
