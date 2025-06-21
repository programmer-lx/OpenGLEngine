#include "ModelRenderer.h"

#include "AssetClass/Model.h"
#include "Core/Subsystems/RenderPipelineSubsystem.h"
#include "Misc/Debug.h"

std::shared_ptr<ModelRenderer> ModelRenderer::create(const std::shared_ptr<Model>& model,
                                                     const std::shared_ptr<Material>& material)
{
    std::shared_ptr<ModelRenderer> ptr(new ModelRenderer);

    ptr->m_Model = model;
    ptr->setMaterial(material);

    RenderPipelineSubsystem::instance()->addRenderer(ptr);

    return ptr;
}

void ModelRenderer::render() const
{
    m_Model->draw();
}

void ModelRenderer::onDestroy()
{
    RenderPipelineSubsystem::instance()->removeRenderer(this);
    Debug::print("ModelRenderer destroy");
}
