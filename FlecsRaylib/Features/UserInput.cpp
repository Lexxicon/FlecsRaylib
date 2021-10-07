#include "UserInput.h"

#include "Vector2.hpp"
#include "Core/CoreTypes.h"
#include "Data/Inputs.h"
#include "Data/Visuals.h"

void UserInput::RegisterTypes(flecs::world& ecs)
{
    ecs.component<SpawnCircleAtMouse>()
        .set<MouseBinding>({
            MouseButton::MOUSE_BUTTON_LEFT
        });

    ecs.component<MoveLeft>()
        .set<KeyBinding>({
            KeyboardKey::KEY_A
        });

}

void UserInput::RegisterSystems(flecs::world& ecs)
{   
    ecs.observer()
        .term<SpawnCircleAtMouse>()
        .event(flecs::OnAdd)
        .iter([](flecs::iter& Iter)
        {
            printf("Clicked\n");
            for(auto i : Iter)
            {
                printf("%s\n", Iter.entity(i).str().c_str());
            }
        });

    ecs.observer()
        .term<MoveLeft>()
        .event(flecs::OnAdd)
        .iter([](flecs::iter& Iter)
        {
            printf("Move Left\n");
            for(auto i : Iter)
            {
                printf("%s\n", Iter.entity(i).str().c_str());
            }
        });
}

void UserInput::InitGlobals(flecs::world& ecs)
{
    ecs.component<MouseInfo>()
        .set<Circle>({WHITE, 5});
}

void UserInput::UpdateMouse(flecs::iter& Iter, MouseInfo* MouseInfos, raylib::Vector2* Positions)
{
    auto MousePosition = GetMousePosition();
    for(auto i : Iter)
    {
    }
}
