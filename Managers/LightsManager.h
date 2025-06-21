#pragma once

#include "ManagerBase.h"
#include "Component/Light/DirectionalLight.h"
#include "Component/Light/PointLight.h"
#include "Component/Light/SpotLight.h"
#include "Graphic/BufferObject.h"

#include <memory>
#include <vector>
#include <map>

class LightsManager : public ManagerBase<LightsManager>
{
private:
    static constexpr std::int32_t POINT_LIGHT_MAX_COUNT = 64;
    static constexpr std::int32_t DIR_LIGHT_MAX_COUNT = 2;
    static constexpr std::int32_t SPOT_LIGHT_MAX_COUNT = 2;

    std::unique_ptr<BufferObject> m_SSBO_PointLights;
    std::unique_ptr<BufferObject> m_SSBO_SpotLights;
    std::unique_ptr<BufferObject> m_SSBO_DirLights;
    std::unique_ptr<BufferObject> m_UBO_LightCount;

    std::vector<DirectionalLight*> m_DirLights;
    std::vector<PointLight*> m_PointLights;
    std::vector<SpotLight*> m_SpotLights;

    std::int32_t m_ShadowPointLightIndex = 0;
    PointLight* m_ShadowMapPointLight = nullptr;

    std::int32_t m_ShadowDirLightIndex = 0;
    DirectionalLight* m_ShadowMapDirLight = nullptr;

    void resetLightCount();

public:
    LightsManager();
    virtual ~LightsManager() override;

    void addDirLight(DirectionalLight* light);
    void removeDirLight(DirectionalLight* light);
    void resetDirLightBuffer();
    DirectionalLight* shadowMapDirLight() const { return m_ShadowMapDirLight; }

    void addPointLight(PointLight* light);
    void removePointLight(PointLight* light);
    void resetPointLightBuffer();
    PointLight* shadowMapPointLight() const { return m_ShadowMapPointLight; }

    void addSpotLight(SpotLight* light);
    void removeSpotLight(SpotLight* light);
    void resetSpotLightBuffer();
};
