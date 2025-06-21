#include "Time.h"

std::int32_t Time::m_MaxFixedTickCount_PerFrame = 5;
float Time::m_UnscaledFixedDeltaTime = 0.02f;
float Time::m_FixedDeltaTime = 0.02f;

float Time::m_Time = 0.0f;
float Time::m_UnscaledTime = 0.0f;

float Time::m_DeltaTime = 0.0f;
float Time::m_UnscaledDeltaTime = 0.0f;

float Time::m_TimeScale = 1.0f;
