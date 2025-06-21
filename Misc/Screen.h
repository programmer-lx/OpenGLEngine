#pragma once

#include <cinttypes>

class Screen
{
private:
    static std::int32_t m_Width;
    static std::int32_t m_Height;

public:
    static std::int32_t width()
    {
        return m_Width;
    }

    static std::int32_t height()
    {
        return m_Height;
    }

    static void notifyResize(std::int32_t width, std::int32_t height);

    /**
     * @return aspect (width / height)
     */
    static float aspect()
    {
        return (float)m_Width / (float)m_Height;
    }
};
