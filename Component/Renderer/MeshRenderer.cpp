#include "MeshRenderer.h"

#include "Component/Camera.h"

#include "AssetClass/Mesh.h"
#include "Core/GameObject.h"
#include "Render/Material.h"
#include "Misc/Debug.h"
#include "Core/Subsystems/RenderPipelineSubsystem.h"

#include <glm/gtc/matrix_transform.hpp>

std::shared_ptr<MeshRenderer> MeshRenderer::create(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material)
{
    std::shared_ptr<MeshRenderer> ptr(new MeshRenderer);
    ptr->setMesh(mesh);
    ptr->setMaterial(material);
    RenderPipelineSubsystem::instance()->addRenderer(ptr);
    return ptr;
}

MeshRenderer::~MeshRenderer()
{
    Debug::print("MeshRenderer destruct");
}

void MeshRenderer::setMesh(const std::shared_ptr<Mesh>& mesh)
{
    m_Mesh = mesh;
}

void MeshRenderer::render() const
{
    // draw
    m_Mesh->draw();
}

void MeshRenderer::onDestroy()
{
    RenderPipelineSubsystem::instance()->removeRenderer(this);
    Debug::print("MeshRenderer destroy");
}
