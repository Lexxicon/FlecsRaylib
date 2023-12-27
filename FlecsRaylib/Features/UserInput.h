#pragma once

#include "Vector2.hpp"
#include "Core/FeatureLifecycle.h"

class UserInput : public FeatureLifecycle<UserInput>
{
public:
    static void RegisterTypes(flecs::world& ecs);
    static void RegisterSystems(flecs::world& ecs);
    static void InitGlobals(flecs::world& ecs);
};
