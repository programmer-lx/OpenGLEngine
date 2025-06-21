#pragma once

#include <cmath>

class Time
{
    friend class TimeSubsystem;

private:
    static std::int32_t m_MaxFixedTickCount_PerFrame;
    static float m_UnscaledFixedDeltaTime;
    static float m_FixedDeltaTime;

    static float m_Time;
    static float m_UnscaledTime;

    static float m_DeltaTime;
    static float m_UnscaledDeltaTime;

    static float m_TimeScale;

public:
    static std::int32_t maxFixedTickCountPerFrame()
    {
        return m_MaxFixedTickCount_PerFrame;
    }

    static void setMaxFixedTickCountPerFrame(std::int32_t maxCount)
    {
        m_MaxFixedTickCount_PerFrame = std::max(1, maxCount);
    }

    static float unscaledFixedDeltaTime()
    {
        return m_UnscaledFixedDeltaTime;
    }

    static void setUnscaledFixedDeltaTime(float unscaledFixedDeltaTime)
    {
        m_UnscaledFixedDeltaTime = std::max(0.002f, unscaledFixedDeltaTime);
    }

    static float fixedDeltaTime()
    {
        return m_FixedDeltaTime;
    }

    static float time()
    {
        return m_Time;
    }

    static float unscaledTime()
    {
        return m_UnscaledTime;
    }

    static float deltaTime()
    {
        return m_DeltaTime;
    }

    static float unscaledDeltaTime()
    {
        return m_UnscaledDeltaTime;
    }

    static float timeScale()
    {
        return m_TimeScale;
    }

    static void setTimeScale(float timeScale)
    {
        m_TimeScale = std::max(0.0f, timeScale);

        // update fixed delta time
        m_FixedDeltaTime = m_UnscaledFixedDeltaTime * m_TimeScale;
    }
};
