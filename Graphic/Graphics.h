#pragma once

#include "VertexArrayObject.h"
#include "BufferObject.h"

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <memory>
#include <string>
#include <functional>

class Shader;
class Material;
class RenderTarget;

class Graphics
{
private:
    static float s_ScreenQuadVertices[24];
    static std::unique_ptr<VertexArrayObject> s_ScreenVAO;
    static std::unique_ptr<BufferObject> s_ScreenVBO;

public:
    static void init();

    static void shutdown();

    static void clearAllTexturesBinding();

    static float* screenQuadVertices(std::uint64_t* bufferSize);

    static void blit(const std::shared_ptr<RenderTarget>& source,
                     const std::shared_ptr<RenderTarget>& destination,
                     GLenum bufferBit = GL_COLOR_BUFFER_BIT,
                     GLenum filterMode = GL_LINEAR);

    static void blit(const std::shared_ptr<RenderTarget>& destination,
                     const std::shared_ptr<Shader>& shader,
                     const std::function<void()>& setUniformsAction);

    static void blit(std::uint32_t destinationFBO, std::int32_t fboWidth, std::int32_t fboHeight,
                     const std::shared_ptr<Shader>& shader,
                     const std::function<void()>& setUniformsAction);
};
