#pragma once

#include "SubsystemBase.h"

#include <cinttypes>

class TimeSubsystem : public SubsystemBase<TimeSubsystem>
{
private:
    std::uint64_t m_StartTime{};
    std::uint64_t m_EndTime{};
    std::uint64_t m_Frequency{};
    double m_MaxFrameRate = 360;

public:
    TimeSubsystem();
    virtual ~TimeSubsystem() override;
    virtual void tick() override;
};
