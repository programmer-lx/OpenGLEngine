#include "HDRISkybox.h"

#include "Render/Shader.h"
#include "Core/Application.h"
#include "Component/Camera.h"

void HDRISkybox::render(Camera* camera)
{
    if (!m_Shader || !m_Shader->isValid())
        return;

    glm::mat4x4 matrixVP = camera->projectionMatrix() * glm::mat4x4(glm::mat3x3(camera->viewMatrix()));

    m_Shader->useProgram();
    m_Shader->setFloat3("skyboxColorMultiply", m_ColorMultiply);
    m_Shader->setMatrix4x4("skyboxMatrixVP", matrixVP);
    m_Shader->setTexture("skybox", m_Tex2D);

    glDepthFunc(GL_LEQUAL);

    // draw
    m_VAO->bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

std::shared_ptr<HDRISkybox> HDRISkybox::create(const std::shared_ptr<Texture2D>& tex)
{
    std::shared_ptr<HDRISkybox> ptr(new HDRISkybox);

    ptr->m_Tex2D = tex;
    ptr->m_Shader = Shader::create(Application::shadersPath() / "FlxShader/HDRISkybox");

    return ptr;
}
