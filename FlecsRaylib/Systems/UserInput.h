#pragma once

#include "../Core/SystemLifecycle.h"

struct MouseInfo;

class UserInput : public SystemLifecycle<UserInput>
{
public:
    static void RegisterTypes(flecs::world& ecs);
    static void RegisterSystems(flecs::world& ecs);
    static void InitGlobals(flecs::world& ecs);
private:
    static void UpdateMouse(flecs::iter& Iter, MouseInfo* MouseInfos);
};
