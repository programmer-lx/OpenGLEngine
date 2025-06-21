#include "Graphics.h"

#include "AssetClass/RenderTarget.h"
#include "Misc/Screen.h"
#include "Misc/Debug.h"
#include "Render/Material.h"

float Graphics::s_ScreenQuadVertices[24] = {
    // positions   // texCoords
    -1.0f, 1.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f,
    1.0f, -1.0f, 1.0f, 0.0f,

    -1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, -1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 1.0f
};

std::unique_ptr<VertexArrayObject> Graphics::s_ScreenVAO{};
std::unique_ptr<BufferObject> Graphics::s_ScreenVBO{};

void Graphics::init()
{
    s_ScreenVAO = std::make_unique<VertexArrayObject>();
    s_ScreenVAO->bind();
    s_ScreenVBO = std::make_unique<BufferObject>(GL_ARRAY_BUFFER, sizeof(s_ScreenQuadVertices), s_ScreenQuadVertices);
    s_ScreenVBO->bind();

    // vertex attribute
    // vertex attribute
    s_ScreenVAO->setLayoutAttribute(0, 2, sizeof(float) * 4, 0);
    s_ScreenVAO->setLayoutAttribute(1, 2, sizeof(float) * 4, sizeof(float) * 2);

    // unbind
    s_ScreenVAO->unbind();
    s_ScreenVBO->unbind();
}

void Graphics::shutdown()
{
    s_ScreenVAO = nullptr;
    s_ScreenVBO = nullptr;
}

void Graphics::clearAllTexturesBinding()
{
    glUseProgram(0);

    for (int i = GL_TEXTURE0; i <= GL_TEXTURE31; ++i)
    {
        glActiveTexture(i);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        glBindTexture(GL_TEXTURE_3D, 0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    }
    glActiveTexture(GL_TEXTURE0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

float* Graphics::screenQuadVertices(std::uint64_t* bufferSize)
{
    *bufferSize = sizeof(s_ScreenQuadVertices);
    return s_ScreenQuadVertices;
}

void Graphics::blit(const std::shared_ptr<RenderTarget>& source,
                    const std::shared_ptr<RenderTarget>& destination,
                    GLenum bufferBit, GLenum filterMode)
{
    source->bindRead();
    std::int32_t destWidth;
    std::int32_t destHeight;

    if (destination == nullptr)
    {
        RenderTarget::bindDefaultWrite();
        destWidth = Screen::width();
        destHeight = Screen::height();
    }
    else
    {
        destination->bindWrite();
        destWidth = destination->width();
        destHeight = destination->height();
    }

    glBlitFramebuffer(0, 0, source->width(), source->height(),
                      0, 0, destWidth, destHeight,
                      bufferBit, filterMode);
    RenderTarget::bindDefault();

    clearAllTexturesBinding();
}

void Graphics::blit(const std::shared_ptr<RenderTarget>& destination,
                    const std::shared_ptr<Shader>& shader,
                    const std::function<void()>& setUniformsAction)
{
    if (destination == nullptr)
    {
        glDisable(GL_DEPTH_TEST); // important, because the screen quad's depth is 0.0f

        glViewport(0, 0, Screen::width(), Screen::height());

        RenderTarget::bindDefaultWrite();
        glClear(GL_COLOR_BUFFER_BIT);

        shader->useProgram();
        if (setUniformsAction)
            setUniformsAction();
        s_ScreenVAO->bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        s_ScreenVAO->unbind();

        glEnable(GL_DEPTH_TEST);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);

        glViewport(0, 0, destination->width(), destination->height());

        destination->bindWrite();
        glClear(GL_COLOR_BUFFER_BIT);

        shader->useProgram();
        if (setUniformsAction)
            setUniformsAction();
        s_ScreenVAO->bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        s_ScreenVAO->unbind();

        glEnable(GL_DEPTH_TEST);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // clearAllTexturesBinding();
}

void Graphics::blit(std::uint32_t destinationFBO, std::int32_t fboWidth, std::int32_t fboHeight,
                    const std::shared_ptr<Shader>& shader,
                    const std::function<void()>& setUniformsAction)
{
    glDisable(GL_DEPTH_TEST);

    glViewport(0, 0, fboWidth, fboHeight);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destinationFBO);

    shader->useProgram();
    if (setUniformsAction)
        setUniformsAction();
    s_ScreenVAO->bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    s_ScreenVAO->unbind();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glEnable(GL_DEPTH_TEST);
}
