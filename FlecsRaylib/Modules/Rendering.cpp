#include "Rendering.h"

#include <array>

#include "RaylibBridge.h"
#include "Core/Reflection.h"
#include "Data/GameTypes.h"
#include "Data/Visuals.h"

Rendering::Rendering(flecs::world& world)
{
    world.import<RaylibBridge>();
    
    std::array RenderPhases = {
        world.entity<RenderPhases::PreDraw>(),
        world.entity<RenderPhases::Background>(),
        world.entity<RenderPhases::Draw>(),
        world.entity<RenderPhases::PostDraw>()
    };

    flecs::entity_t PriorPhase = flecs::OnStore;

    for(auto& Phase : RenderPhases)
    {
        Phase.add(flecs::Phase).depends_on(PriorPhase);
        PriorPhase = Phase;
    }

    Reflection::Register<Position>(world);
    Reflection::Register<Circle>(world);
    
    Reflection::Register<WindowTitle>(world);
    Reflection::Register<WindowSize>(world);
    Reflection::Register<WindowFPS>(world);
    
    world.observer<const WindowTitle>("Update Window Title")
        .event(flecs::OnSet)
        .each([](const WindowTitle& Title)
        {
            if(IsWindowReady())
            {
                SetWindowTitle(Title.Title.c_str());
            }
        });
    world.observer<WindowSize>("Update Window Size")
        .event(flecs::OnSet)
        .each([](const WindowSize& Size)
        {
            if(IsWindowReady())
            {
                SetWindowSize(Size.dimension.x, Size.dimension.y);
            }
        });
    world.observer<WindowFPS>("Update Target FPS")
        .event(flecs::OnSet)
        .each([](const WindowFPS& FPS)
        {
            if(IsWindowReady())
            {
                SetTargetFPS(FPS.Target);
            }
        });
    world.system<const WindowTitle, const WindowSize, const WindowFPS>("InitializeWindow")
        .kind(flecs::OnStart)
        .each([](const WindowTitle& Title, const WindowSize& Size, const WindowFPS& Fps) {
            InitWindow(Size.dimension.x, Size.dimension.y, Title.Title.c_str());
            SetTargetFPS(Fps.Target);
        });


    world.system("Check Exit Request")
        .kind(flecs::PostFrame)
        .iter([](flecs::iter& Iter)
        {
            if(WindowShouldClose())
            {
                Iter.world().quit();
            }
        });
    world.system("BeginDrawing")
        .kind<RenderPhases::PreDraw>()
        .iter([](flecs::iter& Iter)
        {
            BeginDrawing();
            ClearBackground(BLACK);
        });
    world.system("EndDraw")
        .kind<RenderPhases::PostDraw>()
        .iter([](flecs::iter& Iter)
        {
            DrawFPS(10, 10);
            EndDrawing();
        });

    world.system<const Position, const Circle>("Draw Circles")
        .kind<RenderPhases::Draw>()
        .each([](const Position& P, const Circle& C)
        {
            DrawCircleV(P.Value, C.Radius, C.Color);
        });
}
