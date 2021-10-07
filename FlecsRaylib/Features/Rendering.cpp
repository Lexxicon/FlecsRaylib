#include "Rendering.h"

#include "flecs.h"
#include "raylib.h"
#include "Vector2.hpp"
#include "Data/Visuals.h"


void Rendering::RegisterSystems(flecs::world& ecs)
{   
    ecs.system<const raylib::Vector2, const Circle>("Draw Circles")
        .kind(flecs::type_id<RenderPhases::Draw>())
        .iter(DrawCircles);
}

void Rendering::InitGlobals(flecs::world& ecs)
{
    ecs.component<MainWindow>()
        .add<Window>()
        .set<WindowSize>({{800, 450}})
        .set<WindowTitle>({"Basic Window"})
        .set<WindowFPS>({60});
}

void Rendering::DrawCircles(flecs::iter& Iter, const raylib::Vector2* positions, const Circle* circles)
{
    for(auto i : Iter)
    {
        DrawCircleV(positions[i], circles[i].Radius, circles[i].Color);
    }
}
