#include "InputProcessing.h"

#include "InputBindings.h"
#include "Vector2.hpp"

InputProcessing::InputProcessing(flecs::world& world)
{
    world.import<Input>();
    
    world.singleton<Input::MouseInfo>()
        .set<Input::MouseInfo>({})
        .set<raylib::Vector2>({GetMousePosition()});

    world.system<Input::MouseInfo, raylib::Vector2>("Update Mouse Position")
        .kind(flecs::PreFrame)
        .iter([](flecs::iter& Iter, Input::MouseInfo* MouseInfos, raylib::Vector2* Positions)
        {
            const auto MousePosition = GetMousePosition();
            for(auto i : Iter)
            {
                MouseInfos[i].PosDelta = Positions[i] - MousePosition;
                Positions[i] = MousePosition;
            }
        });

    world.system<const Input::MouseBinding>("Trigger Mouse Bindings")
        .kind(flecs::PreFrame)
        .each([](flecs::entity e, const Input::MouseBinding& binding)
        {
            if(IsMouseButtonDown(binding.Bind)){
                e.add(e);
            } else {
                e.remove(e);
            }
        });
    
    world.system<const Input::KeyBinding>("Trigger Key Bindings")
        .kind(flecs::PreFrame)
        .each([](flecs::entity e, const Input::KeyBinding& binding)
        {
            if(IsKeyDown(binding.Key)){
                e.add(e);
            } else {
                e.remove(e);
            }
        });

    using PositiveBind = flecs::pair<Input::KeyBinding, Input::AxisBind::Positive>;
    using NegativeBind = flecs::pair<Input::KeyBinding, Input::AxisBind::Negative>;
    
    world.system<Input::AxisValue, const PositiveBind, const NegativeBind>("Trigger Key Axis Binds")
        .kind(flecs::PreFrame)
        .each([](Input::AxisValue& Axis, const Input::KeyBinding& Positive, const Input::KeyBinding& Negative)
        {
            Axis.Value = IsKeyDown(Positive.Key) ? 1.f : 0.f;
            Axis.Value -= IsKeyDown(Negative.Key) ? 1.f : 0.f;
        });
    
}
