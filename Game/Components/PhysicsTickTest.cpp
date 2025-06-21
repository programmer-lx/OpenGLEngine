#include "PhysicsTickTest.h"
#include "Core/Time.h"
#include "Core/Transform.h"

std::shared_ptr<PhysicsTickTest> PhysicsTickTest::create()
{
    return std::shared_ptr<PhysicsTickTest>(new PhysicsTickTest);
}

void PhysicsTickTest::fixedTick()
{
    speed += Time::fixedDeltaTime() * -9.81f;
    transform().localTranslate(Transform::worldUp() * Time::fixedDeltaTime() * speed);
}
