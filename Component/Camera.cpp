#include "Camera.h"

#include "Core/GameObject.h"
#include "AssetClass/RenderTarget.h"
#include "Render/Shader.h"
#include "Component/PostProcessing.h"
#include "Skybox.h"
#include "Misc/Screen.h"
#include "Misc/Debug.h"
#include "Managers/CameraManager.h"

#include <imgui/imgui.h>

Camera::Camera(std::int32_t depth)
    : ComponentBase()
{
    m_Depth = depth;
}

std::shared_ptr<Camera> Camera::create(std::int32_t depth)
{
    std::shared_ptr<Camera> camera(new Camera(depth));
    CameraManager::instance().addCamera(camera);
    return camera;
}

Camera::~Camera()
{
    Debug::print("Camera destruct");
}

void Camera::awake()
{
}

void Camera::tick()
{
    // m_DeferredLightingShader->setBool("enableShadowMap", enableShadowMap);
    // m_DeferredLightingShader->setBool("enableSSAO", enableSSAO);
    // m_DeferredLightingShader->setBool("enableFog", enableFog);
}

void Camera::onDestroy()
{
    CameraManager::instance().removeCamera(componentID());
    Debug::print("Camera destroy");
}

void Camera::onInspectorGUI()
{
    ComponentBase::onInspectorGUI();

    ImGui::Checkbox("Perspective", &m_IsPerspective);
    ImGui::DragFloat("near", &m_Near, 1, 0.1f, 10000);
    ImGui::DragFloat("far", &m_Far, 1, 0.1f, 10000);
    ImGui::DragFloat("fov", &m_Fov, 1, 1.0f, 179.0f);
    ImGui::DragFloat("half height", &m_HalfHeight);

    // light pass
    // ImGui::Checkbox("Enable Shadow Map", &enableShadowMap);
    // ImGui::Checkbox("Enable SSAO", &enableSSAO);
    // ImGui::Checkbox("Enable Fog", &enableFog);
    // m_DeferredLightingShader->setBool("enableShadowMap", ImGui::Checkbox("Enable Shadow Map", &enableShadowMap));
    // m_DeferredLightingShader->setBool("enableSSAO", ImGui::Checkbox("Enable SSAO", &enableSSAO));
    // m_DeferredLightingShader->setBool("enableFog", ImGui::Checkbox("Enable Fog", &enableFog));

    // Debug::print("ssao: {}", enableSSAO);
}

const std::shared_ptr<Shader>& Camera::deferredLightingShader() const
{
    return m_DeferredLightingShader;
}

void Camera::setDeferredLightingShader(const std::shared_ptr<Shader>& shader)
{
    m_DeferredLightingShader = shader;
}

void Camera::setSsaoShader(const std::shared_ptr<Shader>& shader)
{
    ssaoShader = shader;
}

void Camera::setSsaoBlurShader(const std::shared_ptr<Shader>& shader)
{
    ssaoBlurShader = shader;
}

const glm::vec4& Camera::clearColor() const
{
    return m_ClearColor;
}

void Camera::setClearColor(const glm::vec4& color)
{
    m_ClearColor = color;
}

bool Camera::isPerspective() const
{
    return m_IsPerspective;
}

void Camera::setPerspective(bool isPerspective)
{
    m_IsPerspective = isPerspective;
}

std::int32_t Camera::depth() const
{
    return m_Depth;
}

float Camera::aspect() const
{
    if (m_RenderTarget == nullptr)
        return static_cast<float>(Screen::width()) / static_cast<float>(Screen::height());

    return static_cast<float>(m_RenderTarget->width()) / static_cast<float>(m_RenderTarget->height());
}

float Camera::fov() const
{
    return m_Fov;
}

void Camera::setFov(float fovAngle)
{
    m_Fov = glm::clamp(fovAngle, 0.0f, 179.0f);
}

float Camera::near() const
{
    return m_Near;
}

void Camera::setNear(float near)
{
    m_Near = glm::max(0.1f, near);
}

float Camera::far() const
{
    return m_Far;
}

void Camera::setFar(float far)
{
    m_Far = glm::max(0.1f, far);
}

float Camera::halfHeight() const
{
    return m_HalfHeight;
}

void Camera::setHalfHeight(float height)
{
    m_HalfHeight = glm::max(0.0f, height);
}

const std::shared_ptr<RenderTarget>& Camera::renderTarget() const
{
    return m_RenderTarget;
}

void Camera::setRenderTarget(const std::shared_ptr<RenderTarget>& rt)
{
    m_RenderTarget = rt;
}

PostProcessing* Camera::postProcessing() const
{
    return m_PostProcessing;
}

void Camera::setPostProcessing(PostProcessing* postProcessing)
{
    m_PostProcessing = postProcessing;
}

Skybox* Camera::skybox() const
{
    return m_Skybox;
}

void Camera::setSkybox(Skybox* skybox)
{
    m_Skybox = skybox;
}

glm::mat4x4 Camera::viewMatrix() const
{
    Transform& trans = gameObject()->transform();
    return glm::lookAt(trans.position(), trans.position() + trans.forward(), trans.up());
}

glm::mat4x4 Camera::projectionMatrix() const
{
    if (m_IsPerspective)
        return glm::perspective(glm::radians(m_Fov), aspect(), m_Near, m_Far);

    float halfWidth = Screen::aspect() * m_HalfHeight;
    return glm::ortho(-halfWidth, halfWidth, -m_HalfHeight, m_HalfHeight, m_Near, m_Far);
}

glm::vec3 Camera::screenToWorldPoint(const glm::vec3& screenPoint) const
{
    const glm::vec4 ndcCoords = glm::vec4(
        (screenPoint.x / static_cast<float>(Screen::width())) * 2.0f - 1.0f, // [0, width] -> [-1, 1]
        (screenPoint.y / static_cast<float>(Screen::height())) * 2.0f - 1.0f, // [0, height] -> [-1, 1])
        -1.0f, // this is near plane's coordinate, so z is -1
        1.0f
    );

    glm::vec4 viewCoords = glm::inverse(projectionMatrix()) * ndcCoords;
    viewCoords.z = -1;
    viewCoords.w = 0; // convert to a vector

    const glm::vec4 worldCoords = glm::inverse(viewMatrix()) * viewCoords;
    const glm::vec3 rayDirection = glm::normalize(glm::vec3(worldCoords));

    return transform().position() + rayDirection * screenPoint.z;
}

glm::vec2 Camera::worldToScreenPoint(const glm::vec3& worldPoint) const
{
    glm::vec4 ndcPoint = projectionMatrix() * viewMatrix() * glm::vec4(worldPoint.x, worldPoint.y, worldPoint.z, 1);
    ndcPoint /= ndcPoint.w;

    return glm::vec2(
        (ndcPoint.x + 1.0f) * 0.5f * static_cast<float>(Screen::width()), // [-1, 1] -> [0, width]
        (ndcPoint.y + 1.0f) * 0.5f * static_cast<float>(Screen::height()) // [-1, 1] -> [0, height]
    );
}
