#pragma once

#include "Core/Subsystems/SubsystemBase.h"

class FixedTickSubsystem : public SubsystemBase<FixedTickSubsystem>
{
private:
    float m_AccumulatedTime = 0.0f;

public:
    float accumulatedTime() const;

public:
    virtual void tick() override;
};
