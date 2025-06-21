#pragma once

#include "SubsystemBase.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

#include <vector>

class InputSubsystem : public SubsystemBase<InputSubsystem>
{
private:
    GLFWwindow* m_Window = nullptr;

    // key state:
    // GLFW_RELEASE: Released
    // GLFW_PRESS: Pressed
    static constexpr int KEY_COUNT = GLFW_KEY_LAST + 1;
    int m_CurrentKeyState[KEY_COUNT]{};
    int m_LastKeyState[KEY_COUNT]{};

    // mouse button state
    // GLFW_MOUSE_BUTTON_LEFT: left button
    // GLFW_MOUSE_BUTTON_RIGHT: right button
    // GLFW_MOUSE_BUTTON_MIDDLE: middle button
    static constexpr int MOUSE_BUTTON_COUNT = GLFW_MOUSE_BUTTON_MIDDLE + 1;
    int m_CurrentMouseBtnState[MOUSE_BUTTON_COUNT]{};
    int m_LastMouseBtnState[MOUSE_BUTTON_COUNT]{};

    // mouse scroll
    glm::vec2 m_MouseScrollDelta = glm::vec2(0, 0);

    // mouse movement
    glm::vec2 m_MousePos = glm::vec2(0, 0);
    glm::vec2 m_LastMousePos = glm::vec2(0, 0);

private:
    static glm::vec2 originalMousePosToScreenPos(const glm::vec2& pos);

public:
    explicit InputSubsystem(GLFWwindow* window);

    void notifyScrollValueChanged(double x, double y);
    void setMouseOriginalPosition(double x, double y);
    void notifyMousePositionChanged(double x, double y);

    virtual void tick() override;
    void postTick();

    bool keyDown(int key) const;
    bool keyUp(int key) const;
    bool keyTriggered(int key) const;
    bool multiKeysTriggered(const std::vector<int>& keys) const;
    bool shortcutDown(const std::vector<int>& previousKeys, int currentKey) const;

    bool mouseButtonDown(int button) const;
    bool mouseButtonUp(int button) const;
    bool mouseButtonTriggered(int button) const;

    glm::vec2 mouseScrollDelta() const;

    glm::vec2 mousePosition() const;
    glm::vec2 mousePositionNormalized() const;

    glm::vec2 mousePositionDelta() const;
    glm::vec2 mousePositionDeltaNormalized() const;
};
