#pragma once

#include "Core/Subsystems/SubsystemBase.h"

class SceneSubsystem;
struct GLFWwindow;

class EditorUISubsystem : public SubsystemBase<EditorUISubsystem>
{
private:
    void drawInspectorGUI(SceneSubsystem* sceneSubsystem);

public:
    EditorUISubsystem(GLFWwindow* window);
    virtual ~EditorUISubsystem() override;
    virtual void tick() override;
};
