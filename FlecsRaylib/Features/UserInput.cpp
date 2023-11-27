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
    
    ecs.component<MouseInfo>()
        .add<Position>()
        .set<Circle>({WHITE, 5});
    
    ecs.set<MoveInput>({});
}

void UserInput::RegisterSystems(flecs::world& ecs)
{   
    ecs.observer()
        .term<SpawnCircleAtMouse>()
        .event(flecs::OnAdd)
        .iter([](flecs::iter& Iter)
        {
            auto MousePos = *Iter.world().singleton<MouseInfo>().get<raylib::Vector2>();
            Iter.world().entity()
                .add<ControlledCircle>()
                .set<Circle>({LIME, 10})
                .set<Position>({MousePos});
        });
    
    ecs.system<MoveInput, const AxisChord, const AxisChord>()
        .term_at(2).src<MoveHorizontal>()
        .term_at(3).src<MoveVertical>()
        .each([](MoveInput& Input, const AxisChord& Horizontal, const AxisChord& Vertical)
        {
            raylib::Vector2 MappedInput(Horizontal.Value, Vertical.Value);

            if(MappedInput != raylib::Vector2::Zero())
            {
                MappedInput = MappedInput.Normalize();
                Input.Value = MappedInput;
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
}
