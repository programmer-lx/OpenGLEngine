#pragma once

#include "Renderer.h"

#include "Core/Transform.h"
#include "Graphic/BufferObject.h"

#include <vector>

class Model;

class BatchModelRenderer : public Renderer
{
    FCOMPONENT(BatchModelRenderer)

private:
    std::shared_ptr<Model> m_Model;
    std::vector<Transform> m_Transforms;
    std::unique_ptr<BufferObject> m_InstanceVBO;

public:
    virtual ~BatchModelRenderer();

    static std::shared_ptr<BatchModelRenderer> create(const std::vector<Transform>& transforms,
                                                     const std::shared_ptr<Model>& model,
                                                     const std::shared_ptr<Material>& material);

    virtual void render() const override;

    virtual void onDestroy() override;
};
