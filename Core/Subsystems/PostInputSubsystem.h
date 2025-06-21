#pragma once

#include "InputSubsystem.h"

class PostInputSubsystem : public SubsystemBase<PostInputSubsystem>
{
public:
    virtual void tick() override;
};
