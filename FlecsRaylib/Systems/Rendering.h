#pragma once

#include "../Core/SystemLifecycle.h"
#include "../SubModules/flecs/flecs.h"

class Rendering : public SystemLifecycle<Rendering>
{
public:
    static void RegisterTypes(flecs::world& ecs);
    static void RegisterSystems(flecs::world& ecs);
    static void InitGlobals(flecs::world& ecs);

private:
    static flecs::query<> BuildRenderPipeline(flecs::world& ecs);
};
