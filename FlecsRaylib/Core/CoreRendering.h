#pragma once
#include "FeatureLifecycle.h"
#include "Window.hpp"
#include "Data/Visuals.h"

class CoreRendering : public FeatureLifecycle<CoreRendering>
{
public:
    static void RegisterTypes(flecs::world& ecs);
    static void RegisterSystems(flecs::world& ecs);
    static void InitGlobals(flecs::world& ecs);

private:
    static flecs::query<> BuildRenderPipeline(flecs::world& ecs);
    static int CompareEntityID(
         ecs_entity_t e1, const void *ptr1, 
         ecs_entity_t e2, const void *ptr2);
    static int GetTypeRank(
         flecs::world_t* m_world,
         flecs::type_t m_table_type,
         flecs::entity_t m_grp_type,
         void*);
    
    // Observers
    static void WindowLifecycleHandler(flecs::iter& Iter, Window* windows);
    static void UpdateWindowTitle(flecs::iter& Iter, Window* Windows, const WindowTitle* Titles);
    static void UpdateWindowSize(flecs::iter& Iter, Window* Windows, const WindowSize* Sizes);
    static void UpdateWindowFPS(flecs::iter& Iter, Window* Windows, const WindowFPS* FPS);
    //End Observers

    static void BeginDrawing(flecs::iter& Iter, Window* Windows);
    static void EndDrawing(flecs::iter& Iter, Window* Windows);
};
