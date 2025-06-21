#include "TickSubsystem.h"

#include "Core/Subsystems/SceneSubsystem.h"
#include "Core/GameObject.h"

void TickSubsystem::tick()
{
    SceneSubsystem* sceneSubsystem = SceneSubsystem::instance();

    if (sceneSubsystem)
    {
        // update all gameobjects
        for (const auto& g : sceneSubsystem->gameObjects())
        {
            g->tick();
        }
    }
}
