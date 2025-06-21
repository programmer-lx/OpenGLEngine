#pragma once

#include "Texture.h"

#include <glm/glm.hpp>
#include <memory>

class RenderTarget : public Texture
{
private:
    std::uint32_t m_FBO{}; // FrameBuffer
    std::uint32_t m_RBO{}; // FrameBuffer's RenderBuffer attachment

private:
    RenderTarget();

    void setBuffers(std::int32_t width, std::int32_t height);
    void deleteBuffers();

public:
    virtual ~RenderTarget() override;
    static std::shared_ptr<RenderTarget> create(std::int32_t width, std::int32_t height);

public:
    void bind();
    void bindRead();
    void bindWrite();

    // static
public:
    static void bindDefault();
    static void bindDefaultRead();
    static void bindDefaultWrite();
};
