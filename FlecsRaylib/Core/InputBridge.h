#pragma once
#include "CoreTypes.h"
#include "FeatureLifecycle.h"

class InputBridge : public FeatureLifecycle<InputBridge>
{
public:
    static void RegisterTypes(flecs::world& ecs);
    static void RegisterSystems(flecs::world& ecs);
    static void InitGlobals(flecs::world& ecs);
private:
    static void UpdateMouse(flecs::iter& Iter, MouseInfo* MouseInfos, raylib::Vector2* Positions);
    static void TriggerMouseBindings(flecs::iter& Iter, const MouseBinding* binding);
    static void TriggerKeyBindings(flecs::iter& Iter, const KeyBinding* binding);
};

