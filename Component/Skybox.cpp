#include "Skybox.h"

#include "Camera.h"
#include "Render/Shader.h"
#include "AssetClass/TextureCube.h"
#include "Core/GameObject.h"
#include "Core/Application.h"

#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <imgui/imgui.h>

Skybox::Skybox()
{
    m_VAO = std::make_unique<VertexArrayObject>();
    m_VAO->bind();

    m_VBO = std::make_unique<BufferObject>(GL_ARRAY_BUFFER);
    m_VBO->bind();

    // buffer data
    float skyboxVertices[] = {
        // positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    m_VBO->resetData(sizeof(skyboxVertices), skyboxVertices);

    m_VAO->setLayoutAttribute(0, 3, sizeof(float) * 3, 0);

    m_VBO->unbind();
    m_VAO->unbind();
}

std::shared_ptr<Skybox> Skybox::create(const std::shared_ptr<TextureCube>& tex)
{
    std::shared_ptr<Skybox> ptr(new Skybox);

    ptr->m_TexCube = tex;
    ptr->m_Shader = Shader::create(Application::shadersPath() + "/FlxShader/Skybox");

    return ptr;
}

void Skybox::render(Camera* camera)
{
    if (!m_Shader || !m_Shader->isValid())
        return;

    glm::mat4x4 matrixVP = camera->projectionMatrix() * glm::mat4x4(glm::mat3x3(camera->viewMatrix()));

    m_Shader->useProgram();
    m_Shader->setFloat3("skyboxColorMultiply", m_ColorMultiply);
    m_Shader->setMatrix4x4("skyboxMatrixVP", matrixVP);
    m_Shader->setTexture("skybox", m_TexCube);

    glDepthFunc(GL_LEQUAL);

    // draw
    m_VAO->bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Skybox::awake()
{
    if (Camera* cam = gameObject()->getComponent<Camera>())
    {
        cam->setSkybox(this);
    }
}

void Skybox::onDestroy()
{
    if (Camera* cam = gameObject()->getComponent<Camera>())
    {
        cam->setSkybox(nullptr);
    }
}

void Skybox::onEnable()
{
    if (Camera* cam = gameObject()->getComponent<Camera>())
    {
        cam->setSkybox(this);
    }
}

void Skybox::onDisable()
{
    if (Camera* cam = gameObject()->getComponent<Camera>())
    {
        cam->setSkybox(nullptr);
    }
}

void Skybox::onInspectorGUI()
{
    ComponentBase::onInspectorGUI();
    ImGui::ColorEdit3("skyboxColorMultiply", glm::value_ptr(m_ColorMultiply));
}
