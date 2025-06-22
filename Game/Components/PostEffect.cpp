#include "PostEffect.h"

#include <imgui/imgui.h>

#include "Core/Application.h"

std::shared_ptr<PostEffect> PostEffect::create()
{
    std::shared_ptr<PostEffect> ptr(new PostEffect);
    ptr->m_ColorCorrectionShader = Shader::create(Application::shadersPath() / "FlxShader/PostProcessing/ColorCorrection");
    ptr->m_BlurShader = Shader::create(Application::shadersPath() / "FlxShader/PostProcessing/BloomBlur");
    ptr->m_BrightExtractionShader = Shader::create(Application::shadersPath() / "FlxShader/PostProcessing/BrightExtraction");
    ptr->m_ColorAdditionShader = Shader::create(Application::shadersPath() / "FlxShader/PostProcessing/ColorAddition");

    ptr->m_PingPongRT[0] = RenderTarget::create(1920, 1080);
    ptr->m_PingPongRT[1] = RenderTarget::create(1920, 1080);
    ptr->m_BackRT        = RenderTarget::create(1920, 1080);

    return ptr;
}

void PostEffect::onInspectorGUI()
{
    PostProcessing::onInspectorGUI();

    ImGui::SeparatorText("Color Correction");
    ImGui::DragFloat("gamma", &gamma, 0.01f, -3, 3);
    ImGui::DragFloat("grey", &grey, 0.01f, -3, 3);
    ImGui::DragFloat("contrast", &contrast, 0.01f, -3, 3);

    ImGui::SeparatorText("Bloom");
    ImGui::DragFloat("Bright Threshold", &brightThreshold, 0.001f, 0, 1);
    ImGui::DragInt("Blur Iteration", &blurIteration, 1, 0, 12);
    ImGui::DragFloat("Blur Offset Multiplication", &blurOffsetMultiplication, 0.01f, 0, 10);
}

void PostEffect::process(const std::shared_ptr<RenderTarget>& source)
{
    // color correction pass
    Graphics::blit(m_BackRT, m_ColorCorrectionShader, [&]()
    {
        m_ColorCorrectionShader->setTexture("screenTex", source);
        m_ColorCorrectionShader->setFloat("gamma", gamma);
        m_ColorCorrectionShader->setFloat("grey", grey);
        m_ColorCorrectionShader->setFloat("contrast", contrast);
    });

    // bright extraction pass
    Graphics::blit(m_PingPongRT[0], m_BrightExtractionShader, [&]()
    {
        m_BrightExtractionShader->setTexture("screenTex", m_BackRT);
        m_BrightExtractionShader->setFloat("brightThreshold", brightThreshold);
    });

    // blur pass
    bool isHorizontal = true;
    for (int i = 0; i < blurIteration; ++i)
    {
        Graphics::blit(m_PingPongRT[isHorizontal], m_BlurShader, [&]()
        {
            m_BlurShader->setTexture("screenTex", m_PingPongRT[!isHorizontal]);
            m_BlurShader->setFloat("isHorizontal", isHorizontal);
            m_BlurShader->setFloat("blurOffsetMultiplication", blurOffsetMultiplication);
        });

        isHorizontal = !isHorizontal;
    }

    // color add pass
    Graphics::blit(nullptr, m_ColorAdditionShader, [&]()
    {
        m_ColorAdditionShader->setTexture("tex1", m_PingPongRT[!isHorizontal]);
        m_ColorAdditionShader->setTexture("tex2", m_BackRT);
    });
}

void PostEffect::awake()
{
    PostProcessing::awake();
}
