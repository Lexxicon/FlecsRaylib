#pragma once
#include "CoreTypes.h"
#include "FeatureLifecycle.h"

class CoreRendering : public FeatureLifecycle<CoreRendering>
{
public:
    static void RegisterTypes(flecs::world& ecs);
    static void RegisterSystems(flecs::world& ecs);
    static void InitGlobals(flecs::world& ecs);

private:
    
    // Observers
    static void WindowLifecycleHandler(flecs::iter& Iter, Window* windows);
    static void UpdateWindowTitle(flecs::iter& Iter, Window* Windows, const WindowTitle* Titles);
    static void UpdateWindowSize(flecs::iter& Iter, Window* Windows, const WindowSize* Sizes);
    static void UpdateWindowFPS(flecs::iter& Iter, Window* Windows, const WindowFPS* FPS);
    //End Observers

    static void BeginDrawing(flecs::iter& Iter, Window* Windows);
    static void EndDrawing(flecs::iter& Iter, Window* Windows);
};
