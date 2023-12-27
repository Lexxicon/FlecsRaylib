#include "UserInput.h"

#include "Core/CoreTypes.h"
#include "Data/GameTypes.h"
#include "Data/Inputs.h"
#include "Data/Visuals.h"
#include "Modules/InputBindings.h"

void UserInput::RegisterTypes(flecs::world& ecs)
{
    ecs.component<SpawnCircleAtMouse>()
        .set<Input::MouseBinding>({
            MouseButton::MOUSE_BUTTON_LEFT
        });

    ecs.component<Circle>();
    
    ecs.component<Input::MouseInfo>()
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
            auto MousePos = *Iter.world().singleton<Input::MouseInfo>().get<raylib::Vector2>();
            Iter.world().entity()
                .add<ControlledCircle>()
                .set<Circle>({LIME, 10})
                .set<Position>({MousePos});
        });
    
    ecs.system<MoveInput, const Input::AxisValue, const Input::AxisValue>()
        .term_at(2).src<MoveHorizontal>()
        .term_at(3).src<MoveVertical>()
        .each([](MoveInput& Input, const Input::AxisValue& Horizontal, const Input::AxisValue& Vertical)
        {
            raylib::Vector2 MappedInput(Horizontal.Value, Vertical.Value);

            if(MappedInput != raylib::Vector2::Zero())
            {
                MappedInput = MappedInput.Normalize();
                Input.Value = MappedInput;
            }
            else
            {
                Input.Value = raylib::Vector2::Zero();
            }
        });

    ecs.system<Input::MouseInfo, raylib::Vector2, Position>()
        .kind(flecs::PreFrame)
        .each([](Input::MouseInfo&, raylib::Vector2& v, Position& p)
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
