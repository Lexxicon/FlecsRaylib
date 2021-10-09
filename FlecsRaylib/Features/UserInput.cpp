#include "UserInput.h"

#include "Core/CoreTypes.h"
#include "Data/GameTypes.h"
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
    ecs.component<MoveRight>()
        .set<KeyBinding>({
            KeyboardKey::KEY_D
        });
    ecs.component<MoveUp>()
        .set<KeyBinding>({
            KeyboardKey::KEY_W
        });
    ecs.component<MoveDown>()
        .set<KeyBinding>({
            KeyboardKey::KEY_S
        });

}

void UserInput::RegisterSystems(flecs::world& ecs)
{   
    ecs.observer()
        .term<SpawnCircleAtMouse>()
        .event(flecs::OnAdd)
        .iter([](flecs::iter& Iter)
        {
            for(auto i : Iter)
            {
                Iter.world().entity()
                    .add<ControlledCircle>()
                    .set<Circle>({LIME, 10})
                    .set<Position>({*Iter.world().component<MouseInfo>().get<raylib::Vector2>()});
            }
        });
    
    ecs.system<MoveInput>()
        .term<MoveLeft>().singleton().oper(flecs::Optional)
        .term<MoveRight>().singleton().oper(flecs::Optional)
        .term<MoveUp>().singleton().oper(flecs::Optional)
        .term<MoveDown>().singleton().oper(flecs::Optional)
        .iter([](flecs::iter& Iter, MoveInput* Input)
        {
            raylib::Vector2 MappedInput;
            
            MappedInput.x += Iter.is_set(2) * -1;
            MappedInput.x += Iter.is_set(3) * 1;
            MappedInput.y += Iter.is_set(4) * -1;
            MappedInput.y += Iter.is_set(5) * 1;
            MappedInput = MappedInput.Normalize();

            for(auto i : Iter)
            {
                Input[i].Value = MappedInput;
            }
        });

    ecs.system<MouseInfo, raylib::Vector2, Position>()
        .kind(flecs::PreFrame)
        .each([](MouseInfo&, raylib::Vector2& v, Position& p)
        {
            p.Value = v;
        });
    
    ecs.system<Position, MoveInput>()
        .arg(2).singleton()
        .term<ControlledCircle>()
        .iter([](flecs::iter& Iter, Position* p, MoveInput* input)
        {
            for(auto i : Iter)
            {
                p[i].Value += input->Value * Iter.delta_time() * 100;
            }
        });
}

void UserInput::InitGlobals(flecs::world& ecs)
{
    ecs.component<MouseInfo>()
        .add<Position>()
        .set<Circle>({WHITE, 5});
    ecs.set<MoveInput>({});
}
