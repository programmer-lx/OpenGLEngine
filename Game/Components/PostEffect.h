#pragma once

#include "Component/PostProcessing.h"

class PostEffect : public PostProcessing
{
    FCOMPONENT(PostEffect)

private:
    float gamma = 1.0f;

    float grey = 0.5f;
    float contrast = 1;

    float brightThreshold = 0.3f;

    int blurIteration = 6;
    float blurOffsetMultiplication = 2.6f;

    std::shared_ptr<Shader> m_BrightExtractionShader;
    std::shared_ptr<Shader> m_BlurShader;
    std::shared_ptr<Shader> m_ColorCorrectionShader;
    std::shared_ptr<Shader> m_ColorAdditionShader;

    std::shared_ptr<RenderTarget> m_PingPongRT[2];
    std::shared_ptr<RenderTarget> m_BackRT;

public:
    static std::shared_ptr<PostEffect> create();

    virtual void onInspectorGUI() override;

    virtual void process(const std::shared_ptr<RenderTarget>& source) override;

    virtual void awake() override;
};
