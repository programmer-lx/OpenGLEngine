#include "PBRMaterialParams.h"

#include "Render/Material.h"

#include <imgui/imgui.h>

std::shared_ptr<PBRMaterialParams> PBRMaterialParams::create(class Material* material)
{
    std::shared_ptr<PBRMaterialParams> ptr(new PBRMaterialParams);
    ptr->material = material;
    return ptr;
}

void PBRMaterialParams::onInspectorGUI()
{
    ImGui::DragFloat3("albedoStrength", glm::value_ptr(albedoStrength), 0.01f);
    ImGui::DragFloat("metallicStrength", &metallicStrength, 0.01f);
    ImGui::DragFloat("roughnessStrength", &roughnessStrength, 0.01f);
    ImGui::DragFloat("aoStrength", &aoStrength, 0.01f);
}

void PBRMaterialParams::tick()
{
    if (material)
    {
        material->setFloat3("pbrMaterial.albedoStrength", albedoStrength);
        material->setFloat("pbrMaterial.metallicStrength", metallicStrength);
        material->setFloat("pbrMaterial.roughnessStrength", roughnessStrength);
        material->setFloat("pbrMaterial.aoStrength", aoStrength);
    }
}
