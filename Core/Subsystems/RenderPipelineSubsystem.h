#pragma once

#include "SubsystemBase.h"
#include "Graphic/BufferObject.h"
#include "Component/Light/DirectionalLight.h"
#include "AssetClass/ShadowMap.h"
#include "AssetClass/GBuffer.h"
#include "AssetClass/OmnidirectionalShadowMap.h"
#include "Render/Shader.h"

#include <glm/glm.hpp>

#include <memory>
#include <vector>

constexpr std::int32_t SSAO_KERNEL_COUNT = 16;
struct ShaderSSAOBuffer
{
    glm::vec4 ssaoKernel[SSAO_KERNEL_COUNT];
    glm::vec4 ssaoKernelCount = {SSAO_KERNEL_COUNT, SSAO_KERNEL_COUNT, SSAO_KERNEL_COUNT, SSAO_KERNEL_COUNT};
};

class Renderer;
class Camera;
class Texture2D;

class RenderPipelineSubsystem : public SubsystemBase<RenderPipelineSubsystem>
{
private:
    std::vector<std::shared_ptr<Renderer>> m_OpaqueRenderers_Deferred;
    std::vector<std::shared_ptr<Renderer>> m_OpaqueRenderers_Forward;
    std::vector<std::shared_ptr<Renderer>> m_TransparentRenderers_Forward;

    std::int64_t m_CurShaderID = -1;

    // uniform
    std::unique_ptr<BufferObject> m_UBO_PerCamera; // uniform buffer object
    std::unique_ptr<BufferObject> m_UBO_PerFrame; // uniform buffer object
    glm::mat4x4 m_MatrixM{};
    glm::mat4x4 m_MatrixV{};
    glm::mat4x4 m_MatrixP{};
    glm::mat4x4 m_MatrixVP{};

    // dir light shadow map
    std::shared_ptr<ShadowMap> m_ShadowMap;
    glm::mat4x4 m_DirLightMatrix_VP = glm::mat4x4(1);

    // cube shadow map
    std::shared_ptr<OmnidirectionalShadowMap> m_CubeShadowMap;

    // g-buffer
    std::unique_ptr<GBuffer> m_GBuffer;

    // ssao texture
    std::unique_ptr<BufferObject> m_SSBO_For_SSAO;
    std::uint32_t m_SSAO_FBO{};
    std::uint32_t m_SSAO_Tex_NoBlur{};

    std::uint32_t m_SSAO_Blur_FBO{};
    std::uint32_t m_SSAO_Tex_Blur{};

    std::uint32_t m_SSAONoiseTex{};

    // test SSAO
    // std::shared_ptr<Shader> m_ssao_test_shader;
    // uint32_t tempFBO{};

public:
    // fog
    std::unique_ptr<BufferObject> m_SSBO_Fog;
    glm::vec4 m_FogDensity = {0.0024f, 0, 0, 0};
    glm::vec4 m_FogColor = {0.466, 0.265, 0.466, 1};

    // ambient
    glm::vec4 m_AmbientColor = {0.07, 0.066, 0.1529, 1};

private:
    void setUniformBufferData_PerFrame();

    void setUniformBufferData_PerCamera(Camera* camera);
    void updateSSBO_Fog();

    void renderOneRenderer(const std::shared_ptr<Renderer>& renderer, std::int16_t passID);

    void sortDeferredOpaqueRenderers(const Transform& cameraTransform);
    void sortForwardOpaqueRenderers(const Transform& cameraTransform);
    void sortForwardTransparentRenderers(const Transform& cameraTransform);

    void executeShadowMapPass_DirLight(const std::shared_ptr<Camera>& curCamera);
    void executeShadowMapPass_PointLight(const std::shared_ptr<Camera>& curCamera);
    void executeSsaoPass(const std::shared_ptr<Camera>& curCamera);

    void executeDeferredGeometryPass(const std::shared_ptr<Camera>& curCamera);
    void executeDeferredLightingPass(const std::shared_ptr<Camera>& curCamera);

    void executeForwardBasePass(const std::shared_ptr<Camera>& curCamera);

    void executePostProcessingPass(const std::shared_ptr<Camera>& curCamera);

public:
    explicit RenderPipelineSubsystem();
    virtual ~RenderPipelineSubsystem() override;

    void addRenderer(const std::shared_ptr<Renderer>& renderer);
    void removeRenderer(Renderer* renderer);

    virtual void tick() override;
};
