#pragma once

#include "ComponentBase.h"

#include <glm/gtc/matrix_transform.hpp>

#include <cinttypes>

class RenderTarget;
class Shader;

class Camera : public ComponentBase
{
    friend class PostProcessing;
    friend class Skybox;

    FCOMPONENT(Camera)

    FCOMPONENT_DECLARE_CREATE_FUNC_PARAM1(Camera, std::int32_t, depth)

private:
    bool m_IsPerspective = true;
    std::int32_t m_Depth = -1;

    // common params
    float m_Near = 0.1f;
    float m_Far = 1000.0f;
    glm::vec4 m_ClearColor = glm::vec4(0, 0, 0, 1);

    // perspective params
    float m_Fov = 60.0f;

    // orthometric params
    float m_HalfHeight = 2.0f;

    // RenderTarget & PostProcessing
    std::shared_ptr<RenderTarget> m_RenderTarget;
    PostProcessing* m_PostProcessing = nullptr;

    // skybox
    Skybox* m_Skybox = nullptr;

    // Deferred-Lighting shader
    std::shared_ptr<Shader> m_DeferredLightingShader = nullptr;
    // bool enableShadowMap = true;
    // bool enableSSAO = true;
    // bool enableFog = true;

public:
    std::shared_ptr<Shader> ssaoShader = nullptr;
    std::shared_ptr<Shader> ssaoBlurShader = nullptr;

protected:
    explicit Camera(std::int32_t depth);

protected:
    void setPostProcessing(PostProcessing* postProcessing);
    void setSkybox(Skybox* skybox);

public:
    virtual ~Camera() override;

    virtual void awake() override;
    virtual void tick() override;
    virtual void onDestroy() override;
    virtual void onInspectorGUI() override;

    const std::shared_ptr<Shader>& deferredLightingShader() const;
    void setDeferredLightingShader(const std::shared_ptr<Shader>& shader);

    void setSsaoShader(const std::shared_ptr<Shader>& shader);
    void setSsaoBlurShader(const std::shared_ptr<Shader>& shader);

    const glm::vec4& clearColor() const;
    void setClearColor(const glm::vec4& color);

    bool isPerspective() const;
    void setPerspective(bool isPerspective);

    std::int32_t depth() const;

    /**
     * @return RenderTarget's width/height, if Camera is not possess a RenderTarget object, the RenderTarget is screen
     */
    float aspect() const;

    float fov() const;
    void setFov(float fovAngle);

    float near() const;
    void setNear(float near);

    float far() const;
    void setFar(float far);

    float halfHeight() const;
    void setHalfHeight(float height);

    const std::shared_ptr<RenderTarget>& renderTarget() const;
    void setRenderTarget(const std::shared_ptr<RenderTarget>& rt);

    PostProcessing* postProcessing() const;

    Skybox* skybox() const;

    glm::mat4x4 viewMatrix() const;
    glm::mat4x4 projectionMatrix() const;

    /**
     * @param screenPoint xy: screen position; z: distance that between this point and camera's near plane
     * @return world space point
     */
    glm::vec3 screenToWorldPoint(const glm::vec3& screenPoint) const;

    glm::vec2 worldToScreenPoint(const glm::vec3& worldPoint) const;
};
