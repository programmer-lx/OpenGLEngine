#include "CameraManager.h"

#include "Component/Camera.h"

#include <algorithm>
#include <execution>

void CameraManager::addCamera(const std::shared_ptr<Camera>& camera)
{
    m_Cameras.push_back(camera);

    // sort by camera's depth
    std::sort(std::execution::par, m_Cameras.begin(), m_Cameras.end(), [&](std::shared_ptr<Camera>& a, std::shared_ptr<Camera>& b)
    {
        return a->depth() < b->depth();
    });
}

void CameraManager::removeCamera(std::uint64_t id)
{
    std::erase_if(m_Cameras, [&](const std::shared_ptr<Camera>& cam)
    {
        return cam->componentID() == id;
    });
}
