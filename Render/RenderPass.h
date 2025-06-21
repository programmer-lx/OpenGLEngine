#pragma once

#include "Render/Shader.h"
#include "TestAndBlendDesc.h"

#include <memory>
#include <cinttypes>

struct RenderPass
{
    static constexpr std::int16_t PointLightShadowCaster = 0;
    static constexpr std::int16_t DirLightShadowCaster = 1;
    static constexpr std::int16_t ForwardBase = 2;
    static constexpr std::int16_t ForwardAddition = 3;

    static constexpr std::int16_t DeferredGeometry = 4;
    static constexpr std::int16_t DeferredLighting = 5;

    std::int16_t id = ForwardBase;
    std::shared_ptr<Shader> shader = nullptr;
    CullDesc cullDesc{};
    ColorMask colorMask{};
    BlendDesc blendDesc{};
    DepthDesc depthDesc{};
    StencilDesc stencilDesc{};
};
