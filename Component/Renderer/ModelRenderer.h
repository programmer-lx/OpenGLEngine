#pragma once

#include "Renderer.h"

#include <memory>

class Model;

class ModelRenderer : public Renderer
{
    FCOMPONENT(ModelRenderer)

private:
    std::shared_ptr<Model> m_Model;

private:
    ModelRenderer() = default;

public:
    static std::shared_ptr<ModelRenderer> create(const std::shared_ptr<Model>& model, const std::shared_ptr<Material>& material);
    virtual void render() const override;

public:
    virtual void onDestroy() override;
};
