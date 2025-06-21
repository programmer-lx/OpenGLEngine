#pragma once

#include "Core/Subsystems/SubsystemBase.h"

class TickSubsystem : public SubsystemBase<TickSubsystem>
{
public:
    virtual void tick() override;
};
