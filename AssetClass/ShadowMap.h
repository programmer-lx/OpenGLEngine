#pragma once

#include "Texture.h"
#include "RenderTarget.h"

#include <memory>

class ShadowMap : public Texture
{
private:
    std::uint32_t m_FBO{}; // FrameBuffer

    void deleteBuffers();

public:
    static std::shared_ptr<ShadowMap> create(std::int32_t width, std::int32_t height);
    ShadowMap();
    virtual ~ShadowMap() override;

    void setBuffer(std::int32_t width, std::int32_t height);

    void bindRead();
    void bindWrite();
};
