#pragma once
#include "Core/FeatureLifecycle.h"

class WorldTimeHandler : public FeatureLifecycle<WorldTimeHandler>
{
public:
    static void RegisterTypes(flecs::world& ecs);
    static void RegisterSystems(flecs::world& ecs);
    static void InitGlobals(flecs::world& ecs);

private:
    
};
