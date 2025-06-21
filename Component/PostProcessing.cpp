#include "PostProcessing.h"

#include "Core/GameObject.h"
#include "Component/Camera.h"

void PostProcessing::awake()
{
    if (Camera* cam = gameObject()->getComponent<Camera>())
    {
        cam->setPostProcessing(this);
    }
}

void PostProcessing::onDestroy()
{
    if (Camera* cam = gameObject()->getComponent<Camera>())
    {
        cam->setPostProcessing(nullptr);
    }
}

void PostProcessing::onEnable()
{
    // if (Camera* cam = gameObject()->getComponent<Camera>())
    // {
    //     cam->setPostProcessing(this);
    // }
}

void PostProcessing::onDisable()
{
    // if (Camera* cam = gameObject()->getComponent<Camera>())
    // {
    //     cam->setPostProcessing(nullptr);
    // }
}
