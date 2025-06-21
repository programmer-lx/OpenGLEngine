#include "PostInputSubsystem.h"

void PostInputSubsystem::tick()
{
    InputSubsystem::instance()->postTick();
}
