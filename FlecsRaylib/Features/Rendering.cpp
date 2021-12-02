#include "Rendering.h"

#include "flecs.h"
#include "raylib.h"
#include "Vector2.hpp"
#include "Core/CoreTypes.h"
#include "Data/GameTypes.h"
#include "Data/Visuals.h"


void Rendering::RegisterSystems(flecs::world& ecs)
{   
    ecs.system<const Position, const Circle>("Draw Circles")
        .kind(flecs::type_id<RenderPhases::Draw>())
        .iter(DrawCircles);

    ecs.system<const WindowSize>()
        .arg(1).entity(ecs.component<MainWindow>())
        .term<DayPhase>().oper(flecs::Or)
        .term<NightPhase>().oper(flecs::Or)
        .kind(flecs::type_id<RenderPhases::Draw>())
        .iter([](flecs::iter& Iter, const WindowSize* Window)
        {
            DrawText(Iter.term_id(2) == flecs::type_id<DayPhase>() ?  "Day" : "Night",
                Window->dimension.x/2 - 74, Window->dimension.y/2 + 54, 50, WHITE);
        });
}

void Rendering::InitGlobals(flecs::world& ecs)
{
    ecs.component<MainWindow>()
        .add<Window>()
        .set<WindowSize>({{800, 450}})
        .set<WindowTitle>({"Basic Window"})
        .set<WindowFPS>({60});
}

void Rendering::DrawCircles(flecs::iter& Iter, const Position* positions, const Circle* circles)
{
    for(auto i : Iter)
    {
        DrawCircleV(positions[i].Value, circles[i].Radius, circles[i].Color);
    }
}
