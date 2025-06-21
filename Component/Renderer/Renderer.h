#pragma once

#include "Component/ComponentBase.h"

class Camera;
class Material;

class Renderer : public ComponentBase
{
    friend class RenderPipelineSubsystem;

    FCOMPONENT(Renderer)

private:
    std::shared_ptr<Material> m_Material;

protected:
    const std::shared_ptr<Material>& material() const { return m_Material; }

public:
    void setMaterial(const std::shared_ptr<Material>& material) { m_Material = material; }

public:
    virtual void render() const = 0;
};
