#pragma once

#include "ManagerBase.h"

#include <memory>
#include <vector>

class Camera;

class CameraManager : public ManagerBase<CameraManager>
{
private:
    std::vector<std::shared_ptr<Camera>> m_Cameras;

public:
    void addCamera(const std::shared_ptr<Camera>& camera);
    void removeCamera(std::uint64_t id);

    [[nodiscard]]
    const std::vector<std::shared_ptr<Camera>>& allCameras() const
    {
        return m_Cameras;
    }
};
