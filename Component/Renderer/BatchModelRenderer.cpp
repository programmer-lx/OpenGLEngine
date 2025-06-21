#include "BatchModelRenderer.h"

#include "AssetClass/Model.h"
#include "AssetClass/Mesh.h"
#include "Render/Shader.h"
#include "Render/Material.h"
#include "Core/Subsystems/RenderPipelineSubsystem.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

BatchModelRenderer::~BatchModelRenderer()
{

}

std::shared_ptr<BatchModelRenderer> BatchModelRenderer::create(
    const std::vector<Transform>& transforms,
    const std::shared_ptr<Model>& model,
    const std::shared_ptr<Material>& material)
{
    std::shared_ptr<BatchModelRenderer> ptr(new BatchModelRenderer);

    ptr->m_Model = model;
    ptr->m_Transforms = transforms;
    ptr->setMaterial(material);

    // generate instance vbo
    std::int32_t count = ptr->m_Transforms.size();
    std::vector<glm::mat4x4> modelMatrices(count);
    for (std::int32_t i = 0; i < count; ++i)
    {
        modelMatrices[i] = ptr->m_Transforms[i].modelMatrix();
    }
    ptr->m_InstanceVBO = std::make_unique<BufferObject>(GL_ARRAY_BUFFER, count * sizeof(glm::mat4x4), modelMatrices.data());

    // set vertex attributes
    for (const auto& mesh : ptr->m_Model->meshes())
    {
        mesh->setBatchDrawMatrixM(ptr->m_InstanceVBO.get());
    }

    RenderPipelineSubsystem::instance()->addRenderer(ptr);

    return ptr;
}

void BatchModelRenderer::render() const
{
    // batch render each mesh
    for (const auto& mesh : m_Model->meshes())
    {
        mesh->batchDraw(m_Transforms.size());
    }
}

void BatchModelRenderer::onDestroy()
{
    RenderPipelineSubsystem::instance()->removeRenderer(this);
}
