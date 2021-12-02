#pragma once
#include "Core/FeatureLifecycle.h"
#include "Data/GameTypes.h"

class WorldTimeHandler : public FeatureLifecycle<WorldTimeHandler>
{
public:
    static void RegisterTypes(flecs::world& ecs);
    static void RegisterSystems(flecs::world& ecs);
    static void InitGlobals(flecs::world& ecs);

private:
    static void AdvanceDayPhase(flecs::iter& Iter, TimeLeftInPhase* Countdown, WorldTime* Time, const WorldTimeConfig* TimeConfig);
};
