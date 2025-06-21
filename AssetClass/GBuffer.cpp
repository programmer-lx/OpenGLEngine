#include "GBuffer.h"

#include "Misc/Screen.h"
#include "Misc/Debug.h"

void GBuffer::setBuffers(std::int32_t width, std::int32_t height)
{

}

std::shared_ptr<GBuffer> GBuffer::create(std::int32_t width, std::int32_t height)
{
    std::shared_ptr<GBuffer> ptr(new GBuffer(width, height));
    return ptr;
}

GBuffer::GBuffer(std::int32_t width, std::int32_t height)
{
    if (width < 0 || height < 0)
    {
        m_Width = Screen::width();
        m_Height = Screen::height();
    }
    else
    {
        m_Width = width;
        m_Height = height;
    }

    glGenTextures(4, m_TexIDs.data());
    glGenFramebuffers(1, &m_FBO);

    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

    // gPosition (layout = 0)
    glBindTexture(GL_TEXTURE_2D, m_TexIDs[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TexIDs[0], 0);

    // gNormal (layout = 1)
    glBindTexture(GL_TEXTURE_2D, m_TexIDs[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_Width, m_Height, 0, GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_TexIDs[1], 0);

    // gAlbedo(RGB) + gSpecular(A) (layout = 2)
    glBindTexture(GL_TEXTURE_2D, m_TexIDs[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_TexIDs[2], 0);

    // gEmission(RGB) (layout = 3)
    glBindTexture(GL_TEXTURE_2D, m_TexIDs[3]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_TexIDs[3], 0);

    // set layout
    GLenum attachments[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
    glDrawBuffers(4, attachments);

    // Depth buffer
    glGenRenderbuffers(1, &m_DepthRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, m_DepthRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_Width, m_Height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthRBO);

    // check state
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        Debug::print("G-Buffer error");
    }

    // unbind
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GBuffer::~GBuffer()
{
    glDeleteTextures(4, m_TexIDs.data());
    glDeleteFramebuffers(1, &m_FBO);
    glDeleteRenderbuffers(1, &m_DepthRBO);
}

void GBuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
}

void GBuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
