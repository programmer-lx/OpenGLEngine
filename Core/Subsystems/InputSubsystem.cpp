#include "InputSubsystem.h"

#include "Misc/Screen.h"

#include <cstring>

InputSubsystem::InputSubsystem(GLFWwindow* window)
{
    m_Window = window;

    // init key state
    std::memset(m_CurrentKeyState, GLFW_RELEASE, sizeof(int) * KEY_COUNT);
    std::memset(m_LastKeyState, GLFW_RELEASE, sizeof(int) * KEY_COUNT);

    // init mouse button state
    std::memset(m_CurrentMouseBtnState, GLFW_RELEASE, sizeof(int) * MOUSE_BUTTON_COUNT);
    std::memset(m_LastMouseBtnState, GLFW_RELEASE, sizeof(int) * MOUSE_BUTTON_COUNT);
}

void InputSubsystem::notifyScrollValueChanged(double x, double y)
{
    m_MouseScrollDelta.x = x;
    m_MouseScrollDelta.y = y;
}

void InputSubsystem::setMouseOriginalPosition(double x, double y)
{
    m_MousePos = originalMousePosToScreenPos(glm::vec2(x, y));
    m_LastMousePos = m_MousePos;
}

void InputSubsystem::notifyMousePositionChanged(double x, double y)
{
    m_MousePos = originalMousePosToScreenPos(glm::vec2(x, y));
}

void InputSubsystem::tick()
{
    // keyboard
    for (int i = GLFW_KEY_SPACE; i <= GLFW_KEY_LAST; ++i)
    {
        m_CurrentKeyState[i] = glfwGetKey(m_Window, i);
    }

    // mouse button
    for (int i = GLFW_MOUSE_BUTTON_LEFT; i <= GLFW_MOUSE_BUTTON_MIDDLE; ++i)
    {
        m_CurrentMouseBtnState[i] = glfwGetMouseButton(m_Window, i);
    }
}

void InputSubsystem::postTick()
{
    // keyboard
    std::memcpy(m_LastKeyState, m_CurrentKeyState, sizeof(int) * KEY_COUNT);

    // mouse button
    std::memcpy(m_LastMouseBtnState, m_CurrentMouseBtnState, sizeof(int) * MOUSE_BUTTON_COUNT);

    // mouse scroll
    m_MouseScrollDelta = glm::vec2(0, 0);

    // mouse movement
    m_LastMousePos = m_MousePos;
}

bool InputSubsystem::keyDown(int key) const
{
    return (m_CurrentKeyState[key] != m_LastKeyState[key]) && m_CurrentKeyState[key] == GLFW_PRESS;
}

bool InputSubsystem::keyUp(int key) const
{
    return (m_CurrentKeyState[key] != m_LastKeyState[key]) && m_CurrentKeyState[key] == GLFW_RELEASE;
}

bool InputSubsystem::keyTriggered(int key) const
{
    return m_CurrentKeyState[key] == GLFW_PRESS;
}

bool InputSubsystem::multiKeysTriggered(const std::vector<int>& keys) const
{
    for (const auto& k : keys)
    {
        if (!keyTriggered(k))
            return false;
    }
    return true;
}

bool InputSubsystem::shortcutDown(const std::vector<int>& previousKeys, int currentKey) const
{
    for (const auto& pk : previousKeys)
    {
        if (!keyTriggered(pk))
            return false;
    }

    return keyDown(currentKey);
}

bool InputSubsystem::mouseButtonDown(int button) const
{
    return (m_CurrentMouseBtnState[button] != m_LastMouseBtnState[button]) && m_CurrentMouseBtnState[button] == GLFW_PRESS;
}

bool InputSubsystem::mouseButtonUp(int button) const
{
    return (m_CurrentMouseBtnState[button] != m_LastMouseBtnState[button]) && m_CurrentMouseBtnState[button] == GLFW_RELEASE;
}

bool InputSubsystem::mouseButtonTriggered(int button) const
{
    return m_CurrentMouseBtnState[button] == GLFW_PRESS;
}

glm::vec2 InputSubsystem::mouseScrollDelta() const
{
    return m_MouseScrollDelta;
}

glm::vec2 InputSubsystem::mousePosition() const
{
    return m_MousePos;
}

glm::vec2 InputSubsystem::mousePositionNormalized() const
{
    glm::vec2 result = mousePosition();
    result.x /= static_cast<float>(Screen::width());
    result.y /= static_cast<float>(Screen::height());
    return result;
}

glm::vec2 InputSubsystem::mousePositionDelta() const
{
    return m_MousePos - m_LastMousePos;
}

glm::vec2 InputSubsystem::mousePositionDeltaNormalized() const
{
    glm::vec2 result = mousePositionDelta();
    result.x /= static_cast<float>(Screen::width());
    result.y /= static_cast<float>(Screen::height());
    return result;
}

glm::vec2 InputSubsystem::originalMousePosToScreenPos(const glm::vec2& pos)
{
    glm::vec2 result;
    result.x = pos.x; // x forward: right
    result.y = static_cast<float>(Screen::height()) - pos.y; // y forward: up
    return result;
}
