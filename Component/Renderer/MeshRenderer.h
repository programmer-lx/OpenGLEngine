#pragma once

#include "Renderer.h"

#include <memory>

class Mesh;

class MeshRenderer : public Renderer
{
    FCOMPONENT(MeshRenderer)

private:
    std::shared_ptr<Mesh> m_Mesh;

private:
    MeshRenderer() = default;

    void setMesh(const std::shared_ptr<Mesh>& mesh);

public:
    static std::shared_ptr<MeshRenderer> create(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material);
    virtual ~MeshRenderer() override;

    virtual void render() const override;

public:
    virtual void onDestroy() override;
};
