#pragma once

#include "ComponentBase.h"
#include "Graphic/Graphics.h"
#include "Render/Material.h"
#include "AssetClass/RenderTarget.h"

#include <memory>

class Camera;
class Shader;
class Material;
class RenderTarget;

class PostProcessing : public ComponentBase
{
    // virtual functions
public:
    virtual void process(const std::shared_ptr<RenderTarget>& source) = 0;

public:
    virtual void awake() override;

    virtual void onDestroy() override;

    virtual void onEnable() override;

    virtual void onDisable() override;
};
