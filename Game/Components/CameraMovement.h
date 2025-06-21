#pragma once

#include "Component/ComponentBase.h"

class CameraMovement : public ComponentBase
{
    FCOMPONENT(CameraMovement)

public:
    bool canMove = false;
    float moveSpeed = 30.0f;
    float rotateSpeed = 10000.0f;

public:
    static std::shared_ptr<CameraMovement> create();

    virtual void tick() override;

    virtual void onInspectorGUI() override;
};
