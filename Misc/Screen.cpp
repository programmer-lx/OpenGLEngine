#include "Screen.h"

std::int32_t Screen::m_Width = -1;
std::int32_t Screen::m_Height = -1;

void Screen::notifyResize(std::int32_t width, std::int32_t height)
{
    m_Width = width;
    m_Height = height;
}
