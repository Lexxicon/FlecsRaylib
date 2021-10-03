#pragma once

#include <functional>
#include <vector>

#include "flecs.h"

struct LifecycleHandle
{
    std::function<void(flecs::world&)> RegisterTypes;
    std::function<void(flecs::world&)> RegisterSystems;
    std::function<void(flecs::world&)> InitGlobals;

    static void ProcessHandles(flecs::world& ecs, std::vector<LifecycleHandle>& Handles);
};

template <typename T>
class FeatureLifecycle 
{
public:
    static LifecycleHandle MakeHandle()
    {
        return {
            T::RegisterTypes,
            T::RegisterSystems,
            T::InitGlobals,
        };
    }
    static void RegisterTypes(flecs::world& ecs) {}
    static void RegisterSystems(flecs::world& ecs) {}
    static void InitGlobals(flecs::world& ecs) {}
};
