#pragma once

#include "Component/ComponentBase.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class PBRMaterialParams : public ComponentBase
{
    FCOMPONENT(PBRMaterialParams)

private:
    glm::vec3 albedoStrength = {1, 1, 1};
    float metallicStrength = 1;
    float roughnessStrength = 1;
    float aoStrength = 1;

public:
    static std::shared_ptr<PBRMaterialParams> create(class Material* material);

    Material* material = nullptr;
    virtual void onInspectorGUI() override;
    virtual void tick() override;
};
