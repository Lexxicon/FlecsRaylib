#include "CoreRendering.h"

void CoreRendering::RegisterTypes(flecs::world& ecs)
{
}

void CoreRendering::RegisterSystems(flecs::world& ecs)
{
    // ecs.observer<Window>("Window Lifecycle Manager")
    //     .event(flecs::OnAdd)
    //     .event(flecs::OnRemove)
    //     .iter(WindowLifecycleHandler);
    // ecs.observer<Window, const WindowTitle>("Update Window Title")
    //     .event(flecs::OnSet)
    //     .iter(UpdateWindowTitle);
    // ecs.observer<Window, WindowSize>("Update Window Size")
    //     .event(flecs::OnSet)
    //     .iter(UpdateWindowSize);
    // ecs.observer<Window, WindowFPS>("Update Window FPS")
    //     .event(flecs::OnSet)
    //     .iter(UpdateWindowFPS);
    //
    //
    // ecs.system<Window>("Check Exit Request")
    //     .kind(flecs::PostFrame)
    //     .iter([](flecs::iter& Iter, Window* Windows)
    //     {
    //         for(auto i : Iter)
    //         {
    //             if(Windows[i].hndl->ShouldClose())
    //             {
    //                 Iter.world().quit();
    //             }
    //         }
    //     });
    //
    // ecs.system<Window>("BeginDrawing")
    //     .kind<RenderPhases::PreDraw>()
    //     .iter(BeginDrawing);
    // ecs.system<Window>("EndDraw")
    //     .kind<RenderPhases::PostDraw>()
    //     .iter(EndDrawing);
}

void CoreRendering::InitGlobals(flecs::world& ecs)
{
}

void CoreRendering::WindowLifecycleHandler(flecs::iter& Iter, Window* windows)
{
    if(Iter.event() == flecs::OnAdd)
    {
        for(auto i : Iter)
        {
            windows[i].hndl = new raylib::Window(100, 100);
        }
    }else if(Iter.event() == flecs::OnRemove)
    {
        for(auto i : Iter)
        {
            delete windows[i].hndl;
        }
    }
}

void CoreRendering::UpdateWindowTitle(flecs::iter& Iter, Window* Windows, const WindowTitle* Titles)
{
    if(Iter.event_id() == flecs::type_id<WindowTitle>())
    {
        for(auto i : Iter)
        {
            Windows[i].hndl->SetTitle(Titles[i].Title);
        }
    }
}

void CoreRendering::UpdateWindowSize(flecs::iter& Iter, Window* Windows, const WindowSize* Sizes)
{
    if(Iter.event_id() == flecs::type_id<WindowSize>())
    {
        for(auto i : Iter)
        {
            Windows[i].hndl->SetSize(Sizes[i].dimension);
        }
    }
}

void CoreRendering::UpdateWindowFPS(flecs::iter& Iter, Window* Windows, const WindowFPS* FPS)
{
    if(Iter.event_id() == flecs::type_id<WindowFPS>())
    {
        for(auto i : Iter)
        {
            Windows[i].hndl->SetTargetFPS(FPS[i].Target);
        }
    }
}

void CoreRendering::BeginDrawing(flecs::iter& Iter, Window* Windows)
{
    for(auto i : Iter)
    {
        Windows[i].hndl->BeginDrawing();
        Windows[i].hndl->ClearBackground(BLACK);
    }
}

void CoreRendering::EndDrawing(flecs::iter& Iter, Window* Windows)
{
    for(auto i : Iter)
    {
        DrawFPS(10, 10);
        Windows[i].hndl->EndDrawing();
    }
}