#pragma once

#include "MulticastDelegate.h"

class Timer
{
private:
    MulticastDelegate<> m_Delegates;

    bool m_IsPaused = true;
    bool m_UseTimeScale = true;
    float m_Interval = 0.0f;
    float m_Time = 0.0f;

public:
    Timer(float interval, const std::function<void()>& callback, bool useTimeScale = true, bool startImmediately = true);

    void tick();

    void setUseTimeScale(bool use);
    void setInterval(float interval);
    void addCallback(const std::function<void()>& callback);

    void start();
    void restart();
    void resume();
    void pause();
};
