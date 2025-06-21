#pragma once

#include "Component/ComponentBase.h"

class PhysicsTickTest : public ComponentBase
{
    FCOMPONENT(PhysicsTickTest)

    FCOMPONENT_DECLARE_CREATE_FUNC_PARAM0(PhysicsTickTest)

private:
    float speed = 0;

public:
    virtual void fixedTick() override;
};
