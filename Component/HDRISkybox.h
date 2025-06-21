#pragma once

#include "Skybox.h"
#include "AssetClass/Texture2D.h"

class HDRISkybox : public Skybox
{
private:
    std::shared_ptr<Texture2D> m_Tex2D;

public:
    void render(Camera* camera) override;

    static std::shared_ptr<HDRISkybox> create(const std::shared_ptr<Texture2D>& tex);
};
