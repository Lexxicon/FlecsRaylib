#pragma once

#include "Core/FeatureLifecycle.h"
#include "flecs.h"
#include "Data/GameTypes.h"
#include "Data/Visuals.h"

class Rendering : public FeatureLifecycle<Rendering>
{
public:
    static void RegisterSystems(flecs::world& ecs);
    static void InitGlobals(flecs::world& ecs);

private:
    static void DrawCircles(flecs::iter& Iter, const Position* positions, const Circle* circles);
};
