#pragma once

#include "Core/FeatureLifecycle.h"
#include "flecs.h"

class Rendering : public FeatureLifecycle<Rendering>
{
public:
    static void RegisterTypes(flecs::world& ecs);
    static void RegisterSystems(flecs::world& ecs);
    static void InitGlobals(flecs::world& ecs);

private:
    static flecs::query<> BuildRenderPipeline(flecs::world& ecs);
};
