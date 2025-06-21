#pragma once

#include "Texture.h"

#include <memory>

class OmnidirectionalShadowMap : public Texture
{
private:
    std::uint32_t m_FBO{};

private:
    void setBuffer(std::int32_t width, std::int32_t height);

public:
    static std::shared_ptr<OmnidirectionalShadowMap> create(std::int32_t width, std::int32_t height);
    OmnidirectionalShadowMap();
    virtual ~OmnidirectionalShadowMap() override;

    void bindWrite();
};
