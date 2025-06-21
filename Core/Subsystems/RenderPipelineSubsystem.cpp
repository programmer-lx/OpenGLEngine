#include "RenderPipelineSubsystem.h"

#include "Core/GameObject.h"
#include "Core/Time.h"
#include "Managers/CameraManager.h"
#include "Managers/LightsManager.h"
#include "Component/Renderer/Renderer.h"
#include "Component/Camera.h"
#include "Render/Material.h"
#include "Render/Shader.h"
#include "Render/ShaderUtil.h"
#include "AssetClass/RenderTarget.h"
#include "Component/PostProcessing.h"
#include "Component/Skybox.h"
#include "Misc/Screen.h"
#include "Misc/Debug.h"
#include "Misc/Math.h"
#include "AssetClass/Texture2D.h"
#include "AssetClass/TextureCube.h"
#include "Core/Application.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

#include <algorithm>
#include <execution>


constexpr int TEX_WIDTH = 1920;
constexpr int TEX_HEIGHT = 1080;

void RenderPipelineSubsystem::setUniformBufferData_PerCamera(Camera* camera)
{
    // set uniform buffer data
    m_UBO_PerCamera->bind();
    m_UBO_PerCamera->startAddSubData_BindBufferBase(0);

    // set matrix V and matrix P
    // matrix V
    m_UBO_PerCamera->addSubData<glm::mat4x4>(glm::value_ptr(m_MatrixV));
    // matrix P
    m_UBO_PerCamera->addSubData<glm::mat4x4>(glm::value_ptr(m_MatrixP));
    // matrix VP
    m_UBO_PerCamera->addSubData<glm::mat4x4>(glm::value_ptr(m_MatrixVP));
    // world to view normal matrix
    glm::mat3x3 w2vNormal = glm::mat3x3(glm::transpose(glm::inverse(m_MatrixV)));
    m_UBO_PerCamera->addSubData<glm::mat4x4>(glm::value_ptr(glm::mat4x4(w2vNormal)));
    // camera
    m_UBO_PerCamera->addSubData<glm::vec4>(glm::value_ptr(glm::vec4(camera->near(), camera->far(), camera->fov(), camera->halfHeight())));
    // camera position
    m_UBO_PerCamera->addSubData<glm::vec4>(glm::value_ptr(glm::vec4(camera->transform().position().x,
                                                                    camera->transform().position().y,
                                                                    camera->transform().position().z,
                                                                    1)));
    m_UBO_PerCamera->endAddSubData();
}

void RenderPipelineSubsystem::updateSSBO_Fog()
{
    m_SSBO_Fog->startAddSubData_BindBufferBase(4);
    m_SSBO_Fog->addSubData<glm::vec4>(glm::value_ptr(m_FogDensity));
    m_SSBO_Fog->addSubData<glm::vec4>(glm::value_ptr(m_FogColor));
    m_SSBO_Fog->addSubData<glm::vec4>(glm::value_ptr(m_AmbientColor));
    m_SSBO_Fog->endAddSubData();
}

void RenderPipelineSubsystem::setUniformBufferData_PerFrame()
{
    m_UBO_PerFrame->bind();
    m_UBO_PerFrame->startAddSubData_BindBufferBase(1);

    // time
    m_UBO_PerFrame->addSubData<glm::vec4>(glm::value_ptr(glm::vec4(Time::time(), glm::sin(Time::time()), glm::cos(Time::time()), 1.0f)));
    // screen
    m_UBO_PerFrame->addSubData<glm::vec4>(glm::value_ptr(glm::vec4(Screen::width(),
                                                                   Screen::height(),
                                                                   1.0f / static_cast<float>(Screen::width()),
                                                                   1.0f / static_cast<float>(Screen::height()))));

    m_UBO_PerFrame->endAddSubData();
}

void RenderPipelineSubsystem::renderOneRenderer(const std::shared_ptr<Renderer>& renderer, std::int16_t passID)
{
    auto& material = renderer->material();

    // compute matrix M
    m_MatrixM = renderer->gameObject()->transform().modelMatrix();

    // set matrix object to world normal
    material->setMatrix3x3(ShaderUtil::UNIFORM_NAME_MATRIX_OBJECT_TO_WORLD_NORMAL, glm::transpose(glm::inverse(glm::mat3x3(m_MatrixM))));
    // set matrix object to view normal
    material->setMatrix3x3(ShaderUtil::UNIFORM_NAME_MATRIX_OBJECT_TO_VIEW_NORMAL, glm::transpose(glm::inverse(glm::mat3x3(m_MatrixV * m_MatrixM))));
    // set matrix M
    material->setMatrix4x4(ShaderUtil::UNIFORM_NAME_MATRIX_M, m_MatrixM);
    // set matrix MVP
    material->setMatrix4x4(ShaderUtil::UNIFORM_NAME_MATRIX_MVP, m_MatrixVP * m_MatrixM);


    if (std::vector<RenderPass>* passes = material->renderPasses(passID))
    {
        for (RenderPass& pass : *passes)
        {
            // configure pass
            // if (m_CurShaderID < 0 || m_CurShaderID != pass.shader->shaderID())
            // {
            //     m_CurShaderID = pass.shader->shaderID();
            //     material->useShaderProgram(pass);
            // }
            material->useShaderProgram(pass);
            material->executeAllShaderUniformCommands(pass);
            material->executeTestAndBlendCommands(pass);

            // render pass
            renderer->render();
        }
    }

    // post render commands
    // material->executePostRenderCommands();
}

void RenderPipelineSubsystem::sortDeferredOpaqueRenderers(const Transform& cameraTransform)
{
    // sort by depth, from near to far
    std::sort(std::execution::par, m_OpaqueRenderers_Deferred.begin(), m_OpaqueRenderers_Deferred.end(),
              [&](std::shared_ptr<Renderer>& a, std::shared_ptr<Renderer>& b)
              {
                  float distanceA = glm::length(a->transform().position() - cameraTransform.position());
                  float distanceB = glm::length(b->transform().position() - cameraTransform.position());
                  return distanceA < distanceB;
              });
}

void RenderPipelineSubsystem::sortForwardOpaqueRenderers(const Transform& cameraTransform)
{
    // sort by depth, from near to far
    std::sort(std::execution::par, m_OpaqueRenderers_Forward.begin(), m_OpaqueRenderers_Forward.end(),
              [&](std::shared_ptr<Renderer>& a, std::shared_ptr<Renderer>& b)
              {
                  float distanceA = glm::length(a->transform().position() - cameraTransform.position());
                  float distanceB = glm::length(b->transform().position() - cameraTransform.position());
                  return distanceA < distanceB;
              });
}

void RenderPipelineSubsystem::sortForwardTransparentRenderers(const Transform& cameraTransform)
{
    // sort by depth, from for to near
    std::sort(std::execution::par, m_TransparentRenderers_Forward.begin(), m_TransparentRenderers_Forward.end(),
              [&](std::shared_ptr<Renderer>& a, std::shared_ptr<Renderer>& b)
              {
                  float distanceA = glm::length(a->transform().position() - cameraTransform.position());
                  float distanceB = glm::length(b->transform().position() - cameraTransform.position());
                  return distanceA > distanceB;
              });
}

RenderPipelineSubsystem::RenderPipelineSubsystem()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_BLEND);

    // UBO PerCamera
    // clang-format off
    std::int64_t bufferSize = 0;
    bufferSize += sizeof(glm::mat4x4);  // matrix V                         (mat4x4)
    bufferSize += sizeof(glm::mat4x4);  // matrix P                         (mat4x4)
    bufferSize += sizeof(glm::mat4x4);  // matrix VP                        (mat4x4)
    bufferSize += sizeof(glm::mat4x4);  // world to view normal matrix      (mat3x3)
    bufferSize += sizeof(glm::vec4);    // _camera                          (vec4)
    bufferSize += sizeof(glm::vec4);    // _cameraPos                       (vec3)
    // clang-format on

    // generate uniform buffer
    m_UBO_PerCamera = std::make_unique<BufferObject>(GL_UNIFORM_BUFFER, bufferSize);


    // UBO PerFrame
    // clang-format off
    bufferSize = 0;
    bufferSize += sizeof(glm::vec4);    // _time        (vec4)
    bufferSize += sizeof(glm::vec4);    // _screen      (vec4)
    // clang-format on

    // generate uniform buffer
    m_UBO_PerFrame = std::make_unique<BufferObject>(GL_UNIFORM_BUFFER, bufferSize);

    // G-Buffer
    m_GBuffer = std::make_unique<GBuffer>(TEX_WIDTH, TEX_HEIGHT);

    // SSAO FBO
    // clang-format off
    glGenFramebuffers(1, &m_SSAO_FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_SSAO_FBO);
    glGenTextures(1, &m_SSAO_Tex_NoBlur);
    glBindTexture(GL_TEXTURE_2D, m_SSAO_Tex_NoBlur);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, TEX_WIDTH, TEX_HEIGHT, 0, GL_RGB, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_SSAO_Tex_NoBlur, 0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            Debug::print("SSAO Framebuffer Error");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // clang-format on

    // SSAO Blur FBO
    // clang-format off
    glGenFramebuffers(1, &m_SSAO_Blur_FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_SSAO_Blur_FBO);
    glGenTextures(1, &m_SSAO_Tex_Blur);
    glBindTexture(GL_TEXTURE_2D, m_SSAO_Tex_Blur);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, TEX_WIDTH, TEX_HEIGHT, 0, GL_RGB, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_SSAO_Tex_Blur, 0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            Debug::print("SSAO Blur Framebuffer Error");
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // clang-format on

    // SSAO Noise (4x4 RGB texture)
    constexpr int noiseWidth = 32;
    constexpr int noiseCount = noiseWidth * noiseWidth;
    glm::vec3 ssaoNoise[noiseCount];
    for (int i = 0; i < noiseCount; ++i)
    {
        ssaoNoise[i] = glm::vec3(
            Math::uniformRandom(-1.0f, 1.0f),
            Math::uniformRandom(-1.0f, 1.0f),
            Math::uniformRandom(0.0f, 1.0f)
        );
        ssaoNoise[i] = glm::normalize(ssaoNoise[i]);
    }
    // clang-format off
    glGenTextures(1, &m_SSAONoiseTex);
    glBindTexture(GL_TEXTURE_2D, m_SSAONoiseTex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, noiseWidth, noiseWidth, 0, GL_RGB, GL_FLOAT, glm::value_ptr(ssaoNoise[0]));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    // clang-format on

    // SSBO For SSAO
    ShaderSSAOBuffer ssaoBuffer;
    for (int i = 0; i < ssaoBuffer.ssaoKernelCount.x; ++i)
    {
        // 随机方向向量，落在法向半球以内
        glm::vec3 dir = glm::vec3(
            Math::uniformRandom(-1.0f, 1.0f),
            Math::uniformRandom(-1.0f, 1.0f),
            Math::uniformRandom(0.0f, 1.0f)
        );
        dir = glm::normalize(dir);
        // 随机半径
        float radius = Math::uniformRandom(0.0f, 1.0f);

        // sample vector
        glm::vec3 vec = dir * radius;
        float scale = (float)i / (float)ssaoBuffer.ssaoKernelCount.x;
        scale = std::lerp(0.1f, 1.0f, scale * scale);
        vec *= scale;
        ssaoBuffer.ssaoKernel[i] = glm::vec4(vec, 0);
    }
    m_SSBO_For_SSAO = std::make_unique<BufferObject>(GL_SHADER_STORAGE_BUFFER, sizeof(ShaderSSAOBuffer));
    m_SSBO_For_SSAO->startAddSubData_BindBufferBase(3);
    m_SSBO_For_SSAO->addSubData<ShaderSSAOBuffer>(&ssaoBuffer);
    m_SSBO_For_SSAO->endAddSubData();

    // Cube shadow map
    m_CubeShadowMap = OmnidirectionalShadowMap::create(1024, 1024);

    // dir light shadow map
    // m_ShadowMap = ShadowMap::create(8000, 8000);

    // fog
    m_SSBO_Fog = std::make_unique<BufferObject>(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * 3);
    updateSSBO_Fog();

    // test
    // m_ssao_test_shader = Shader::create(Application::shadersPath() + "/FlxShader/JustRedChannel");
}

RenderPipelineSubsystem::~RenderPipelineSubsystem()
{
    glDeleteFramebuffers(1, &m_SSAO_FBO);
    glDeleteTextures(1, &m_SSAO_Tex_NoBlur);

    glDeleteFramebuffers(1, &m_SSAO_Blur_FBO);
    glDeleteTextures(1, &m_SSAO_Tex_Blur);

    glDeleteTextures(1, &m_SSAONoiseTex);
}

void RenderPipelineSubsystem::addRenderer(const std::shared_ptr<Renderer>& renderer)
{
    switch (renderer->material()->materialType())
    {
        case MaterialType::DeferredOpaque:
            m_OpaqueRenderers_Deferred.push_back(renderer);
            break;

        case MaterialType::ForwardOpaque:
        case MaterialType::ForwardAlphaTest:
            m_OpaqueRenderers_Forward.push_back(renderer);
            break;

        case MaterialType::ForwardTransparent:
            m_TransparentRenderers_Forward.push_back(renderer);
            break;
    }
}

void RenderPipelineSubsystem::removeRenderer(Renderer* renderer)
{
    switch (renderer->material()->materialType())
    {
        case MaterialType::DeferredOpaque:
            std::erase_if(m_OpaqueRenderers_Deferred, [&](const std::shared_ptr<Renderer>& r)
            {
                return r->componentID() == renderer->componentID();
            });
            break;

        case MaterialType::ForwardOpaque:
        case MaterialType::ForwardAlphaTest:
            std::erase_if(m_OpaqueRenderers_Forward, [&](const std::shared_ptr<Renderer>& r)
            {
                return r->componentID() == renderer->componentID();
            });
            break;

        case MaterialType::ForwardTransparent:
            erase_if(m_TransparentRenderers_Forward, [&](const std::shared_ptr<Renderer>& r)
            {
                return r->componentID() == renderer->componentID();
            });
            break;
    }
}

void RenderPipelineSubsystem::executeForwardBasePass(const std::shared_ptr<Camera>& curCamera)
{
    // render Opaque and AlphaTest objects
    sortForwardOpaqueRenderers(curCamera->transform());
    for (const auto& renderer : m_OpaqueRenderers_Forward)
    {
        renderOneRenderer(renderer, RenderPass::ForwardBase);
    }

    // if camera has skybox, render skybox
    if (curCamera->skybox() != nullptr)
    {
        curCamera->skybox()->render(curCamera.get());
    }

    // render transparent objects
    sortForwardTransparentRenderers(curCamera->transform());
    for (const auto& renderer : m_TransparentRenderers_Forward)
    {
        renderOneRenderer(renderer, RenderPass::ForwardBase);
    }
}

void RenderPipelineSubsystem::executeShadowMapPass_DirLight(const std::shared_ptr<Camera>& curCamera)
{
    auto dirLight = LightsManager::instance().shadowMapDirLight();
    if (dirLight == nullptr || m_ShadowMap == nullptr)
        return;

    m_ShadowMap->bindWrite();

    m_DirLightMatrix_VP = dirLight->projectionMatrix() * dirLight->viewMatrix();

    glViewport(0, 0, m_ShadowMap->width(), m_ShadowMap->height());

    glClearColor(1, 1, 1, 1);
    glClear(GL_DEPTH_BUFFER_BIT);

    // render depth map

    // deferred objects
    sortDeferredOpaqueRenderers(curCamera->transform());
    for (const auto& renderer : m_OpaqueRenderers_Deferred)
    {
        renderer->material()->setTexture("shadowMap", m_ShadowMap);
        renderer->material()->setMatrix4x4("_dirLightSpaceMatrix_VP", m_DirLightMatrix_VP);

        renderOneRenderer(renderer, RenderPass::DirLightShadowCaster);
    }

    // render Opaque and AlphaTest objects
    sortForwardOpaqueRenderers(curCamera->transform());
    for (const auto& renderer : m_OpaqueRenderers_Forward)
    {
        renderer->material()->setTexture("shadowMap", m_ShadowMap);
        renderer->material()->setMatrix4x4("_dirLightSpaceMatrix_VP", m_DirLightMatrix_VP);

        renderOneRenderer(renderer, RenderPass::DirLightShadowCaster);
    }

    // render transparent objects
    sortForwardOpaqueRenderers(curCamera->transform());
    for (const auto& renderer : m_TransparentRenderers_Forward)
    {
        renderer->material()->setTexture("shadowMap", m_ShadowMap);
        renderer->material()->setMatrix4x4("_dirLightSpaceMatrix_VP", m_DirLightMatrix_VP);

        renderOneRenderer(renderer, RenderPass::DirLightShadowCaster);
    }
}

void RenderPipelineSubsystem::executeShadowMapPass_PointLight(const std::shared_ptr<Camera>& curCamera)
{
    auto pointLight = LightsManager::instance().shadowMapPointLight();
    if (pointLight == nullptr || m_CubeShadowMap == nullptr)
        return;

    m_CubeShadowMap->bindWrite();

    auto projectionMatrix = pointLight->projectionMatrix();

    glViewport(0, 0, m_CubeShadowMap->width(), m_CubeShadowMap->height());

    glClearColor(1, 1, 1, 1);
    glClear(GL_DEPTH_BUFFER_BIT);

    sortDeferredOpaqueRenderers(curCamera->transform());
    for (const auto& renderer : m_OpaqueRenderers_Deferred)
    {
        renderer->material()->setTexture("cubeShadowMap", m_CubeShadowMap);
        renderer->material()->setFloat("farPlane", pointLight->projectionFar);

        auto matrices = pointLight->cubeViewMatrices();
        renderer->material()->setMatrix4x4("lightSpaceMatrix_VP[0]", projectionMatrix * matrices[0]);
        renderer->material()->setMatrix4x4("lightSpaceMatrix_VP[1]", projectionMatrix * matrices[1]);
        renderer->material()->setMatrix4x4("lightSpaceMatrix_VP[2]", projectionMatrix * matrices[2]);
        renderer->material()->setMatrix4x4("lightSpaceMatrix_VP[3]", projectionMatrix * matrices[3]);
        renderer->material()->setMatrix4x4("lightSpaceMatrix_VP[4]", projectionMatrix * matrices[4]);
        renderer->material()->setMatrix4x4("lightSpaceMatrix_VP[5]", projectionMatrix * matrices[5]);

        renderOneRenderer(renderer, RenderPass::PointLightShadowCaster);
    }

    // render Opaque and AlphaTest objects
    sortForwardOpaqueRenderers(curCamera->transform());
    for (const auto& renderer : m_OpaqueRenderers_Forward)
    {
        renderer->material()->setTexture("cubeShadowMap", m_CubeShadowMap);
        renderer->material()->setFloat("farPlane", pointLight->projectionFar);

        auto matrices = pointLight->cubeViewMatrices();
        renderer->material()->setMatrix4x4("lightSpaceMatrix_VP[0]", projectionMatrix * matrices[0]);
        renderer->material()->setMatrix4x4("lightSpaceMatrix_VP[1]", projectionMatrix * matrices[1]);
        renderer->material()->setMatrix4x4("lightSpaceMatrix_VP[2]", projectionMatrix * matrices[2]);
        renderer->material()->setMatrix4x4("lightSpaceMatrix_VP[3]", projectionMatrix * matrices[3]);
        renderer->material()->setMatrix4x4("lightSpaceMatrix_VP[4]", projectionMatrix * matrices[4]);
        renderer->material()->setMatrix4x4("lightSpaceMatrix_VP[5]", projectionMatrix * matrices[5]);

        renderOneRenderer(renderer, RenderPass::PointLightShadowCaster);
    }

    // render transparent objects
    sortForwardOpaqueRenderers(curCamera->transform());
    for (const auto& renderer : m_TransparentRenderers_Forward)
    {
        renderer->material()->setTexture("cubeShadowMap", m_CubeShadowMap);
        renderer->material()->setFloat("farPlane", pointLight->projectionFar);

        auto matrices = pointLight->cubeViewMatrices();
        renderer->material()->setMatrix4x4("lightSpaceMatrix_VP[0]", projectionMatrix * matrices[0]);
        renderer->material()->setMatrix4x4("lightSpaceMatrix_VP[1]", projectionMatrix * matrices[1]);
        renderer->material()->setMatrix4x4("lightSpaceMatrix_VP[2]", projectionMatrix * matrices[2]);
        renderer->material()->setMatrix4x4("lightSpaceMatrix_VP[3]", projectionMatrix * matrices[3]);
        renderer->material()->setMatrix4x4("lightSpaceMatrix_VP[4]", projectionMatrix * matrices[4]);
        renderer->material()->setMatrix4x4("lightSpaceMatrix_VP[5]", projectionMatrix * matrices[5]);

        renderOneRenderer(renderer, RenderPass::PointLightShadowCaster);
    }
}

void RenderPipelineSubsystem::executeSsaoPass(const std::shared_ptr<Camera>& curCamera)
{
    if (curCamera->ssaoShader != nullptr)
    {
        const std::shared_ptr<Shader> shader = curCamera->ssaoShader;
        Graphics::blit(m_SSAO_FBO, TEX_WIDTH, TEX_HEIGHT, shader, [&]()
        {
            // G-Buffer
            curCamera->ssaoShader->setTexture(ShaderUtil::UNIFORM_NAME_G_POSITION, m_GBuffer->texIDs()[0], GL_TEXTURE_2D);
            curCamera->ssaoShader->setTexture(ShaderUtil::UNIFORM_NAME_G_NORMAL, m_GBuffer->texIDs()[1], GL_TEXTURE_2D);
            curCamera->ssaoShader->setTexture(ShaderUtil::UNIFORM_NAME_G_ALBEDO_SPECULAR, m_GBuffer->texIDs()[2], GL_TEXTURE_2D);
            // SSAO noise Tex
            curCamera->ssaoShader->setTexture(ShaderUtil::UNIFORM_NAME_SSAO_NOISE_TEX, m_SSAONoiseTex, GL_TEXTURE_2D);
        });

        // blur
        if (curCamera->ssaoBlurShader != nullptr)
        {
            const std::shared_ptr<Shader> blurShader = curCamera->ssaoBlurShader;
            Graphics::blit(m_SSAO_Blur_FBO, TEX_WIDTH, TEX_HEIGHT, blurShader, [&]()
            {
                curCamera->ssaoBlurShader->setTexture(ShaderUtil::UNIFORM_NAME_SSAO_TEX_NO_BLUR, m_SSAO_Tex_NoBlur, GL_TEXTURE_2D);
            });
        }
    }
}

void RenderPipelineSubsystem::executeDeferredGeometryPass(const std::shared_ptr<Camera>& curCamera)
{
    // bind
    m_GBuffer->bind();
    glViewport(0, 0, m_GBuffer->width(), m_GBuffer->height());

    // clear
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    sortDeferredOpaqueRenderers(curCamera->transform());
    for (const auto& renderer : m_OpaqueRenderers_Deferred)
    {
        renderOneRenderer(renderer, RenderPass::DeferredGeometry);
    }

    // unbind
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPipelineSubsystem::executeDeferredLightingPass(const std::shared_ptr<Camera>& curCamera)
{
    std::shared_ptr<RenderTarget> target = curCamera->renderTarget();

    const std::shared_ptr<Shader>& shader = curCamera->deferredLightingShader();
    if (shader == nullptr)
        return;

    Graphics::blit(target, shader, [&]()
    {
        shader->setTexture(ShaderUtil::UNIFORM_NAME_G_POSITION, m_GBuffer->texIDs()[0], GL_TEXTURE_2D);
        shader->setTexture(ShaderUtil::UNIFORM_NAME_G_NORMAL, m_GBuffer->texIDs()[1], GL_TEXTURE_2D);
        shader->setTexture(ShaderUtil::UNIFORM_NAME_G_ALBEDO_SPECULAR, m_GBuffer->texIDs()[2], GL_TEXTURE_2D);
        shader->setTexture(ShaderUtil::UNIFORM_NAME_G_EMISSION, m_GBuffer->texIDs()[3], GL_TEXTURE_2D);

        // cube shadow map
        auto pointLight = LightsManager::instance().shadowMapPointLight();
        if (pointLight != nullptr && m_CubeShadowMap != nullptr)
        {
            shader->setTexture("cubeShadowMap", m_CubeShadowMap->texID(), GL_TEXTURE_CUBE_MAP);
            shader->setFloat("farPlane", pointLight->projectionFar);
        }

        // shadow map
        auto dirLight = LightsManager::instance().shadowMapDirLight();
        if (dirLight != nullptr && m_ShadowMap != nullptr)
        {
            shader->setTexture("shadowMap", m_ShadowMap);
            shader->setMatrix4x4("_dirLightSpaceMatrix_VP", m_DirLightMatrix_VP);
        }

        // set SSAO texture
        if (curCamera->ssaoBlurShader != nullptr)
            shader->setTexture(ShaderUtil::UNIFORM_NAME_SSAO_TEX, m_SSAO_Tex_Blur, GL_TEXTURE_2D);
        else
            shader->setTexture(ShaderUtil::UNIFORM_NAME_SSAO_TEX_NO_BLUR, m_SSAO_Tex_NoBlur, GL_TEXTURE_2D);

        // _skyboxTex
        if (curCamera->skybox() != nullptr)
            shader->setTexture("_skyboxTex", std::dynamic_pointer_cast<TextureCube>(curCamera->skybox()->m_TexCube));
    });
}

void RenderPipelineSubsystem::executePostProcessingPass(const std::shared_ptr<Camera>& curCamera)
{
    if (curCamera->renderTarget() != nullptr)
    {
        if (curCamera->postProcessing() != nullptr)
        {
            curCamera->postProcessing()->process(curCamera->renderTarget());
        }
    }
}

void RenderPipelineSubsystem::tick()
{
    LightsManager::instance().resetPointLightBuffer();
    LightsManager::instance().resetDirLightBuffer();
    LightsManager::instance().resetSpotLightBuffer();

    // update fog & ambient
    updateSSBO_Fog();

    auto& allCameras = CameraManager::instance().allCameras();

    // set uniforms per-frame
    setUniformBufferData_PerFrame();

    // for each camera
    const std::int64_t last = static_cast<std::int64_t>(allCameras.size()) - 1;
    for (std::int64_t i = 0; i <= last; ++i)
    {
        const auto& curCamera = allCameras[i];

        // compute matrix V, P
        m_MatrixV = curCamera->viewMatrix();
        m_MatrixP = curCamera->projectionMatrix();
        m_MatrixVP = m_MatrixP * m_MatrixV;

        // set uniforms per-camera
        setUniformBufferData_PerCamera(curCamera.get());

        // Pass: shadow map
        // executeShadowMapPass_DirLight(curCamera);
        executeShadowMapPass_PointLight(curCamera);

        // -------------- Deferred Shading ------------------

        // Pass: geometry
        executeDeferredGeometryPass(curCamera);

        // Pass: SSAO
        executeSsaoPass(curCamera);

        // Temp: show SSAO
        // Graphics::blit(0, TEX_WIDTH, TEX_HEIGHT, m_ssao_test_shader, [&]()
        // {
        //     m_ssao_test_shader->setTexture("testSsaoTex", m_SSAO_Tex_NoBlur, GL_TEXTURE_2D);
        // });
        // return;

        // if the camera has RenderTarget, draw scene to the RT
        if (curCamera->renderTarget() != nullptr)
        {
            glViewport(0, 0, curCamera->renderTarget()->width(), curCamera->renderTarget()->height());
            curCamera->renderTarget()->bindWrite();
        }
        else
        {
            glViewport(0, 0, Screen::width(), Screen::height());
            RenderTarget::bindDefaultWrite();
        }

        // Pass: lighting
        // Graphics::clearAllTexturesBinding();
        executeDeferredLightingPass(curCamera);

        // write depth value to default framebuffer
        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_GBuffer->fbo());
        std::int32_t w, h;
        if (curCamera->renderTarget() != nullptr)
        {
            w = curCamera->renderTarget()->width();
            h = curCamera->renderTarget()->height();
            curCamera->renderTarget()->bindWrite();
        }
        else
        {
            w = Screen::width();
            h = Screen::height();
            RenderTarget::bindDefaultWrite();
        }
        glBlitFramebuffer(0, 0, m_GBuffer->width(), m_GBuffer->height(), 0, 0, w, h, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

        // -------------- Forward Shading ------------------

        // Pass: base
        executeForwardBasePass(curCamera);


        // -------------- Post-Processing ------------------

        // Pass: post-processing
        executePostProcessingPass(curCamera);

        Graphics::clearAllTexturesBinding();
    }
}
