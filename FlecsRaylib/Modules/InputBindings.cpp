#include "InputBindings.h"

#include "RaylibBridge.h"
#include "Core/CoreTypes.h"
#include "Data/Visuals.h"

Input::Input(flecs::world& world)
{
    world.import<RaylibBridge>();
    
    Reflection::Register<MouseBinding>(world);
    Reflection::Register<KeyBinding>(world);
    
    Reflection::Register<AxisBind>(world);
    Reflection::Register<AxisBind::Positive>(world);
    Reflection::Register<AxisBind::Negative>(world);
    Reflection::Register<AxisValue>(world);
    
    Reflection::Register<MouseInfo>(world);

    world.singleton<MouseInfo>()
        .set<MouseInfo>({})
        .set<raylib::Vector2>({GetMousePosition()});

    world.system<MouseInfo, raylib::Vector2>("Update Mouse Position")
        .kind(flecs::PreFrame)
        .iter([](flecs::iter& Iter, MouseInfo* MouseInfos, raylib::Vector2* Positions)
        {
            const auto MousePosition = GetMousePosition();
            for(auto i : Iter)
            {
                MouseInfos[i].PosDelta = Positions[i] - MousePosition;
                Positions[i] = MousePosition;
            }
        });

    world.system<const MouseBinding>("Trigger Mouse Bindings")
        .kind(flecs::PreFrame)
        .each([](flecs::entity e, const MouseBinding& binding)
        {
            if(IsMouseButtonDown(binding.Bind)){
                e.add(e);
            } else {
                e.remove(e);
            }
        });
    
    world.system<const KeyBinding>("Trigger Key Bindings")
        .kind(flecs::PreFrame)
        .each([](flecs::entity e, const KeyBinding& binding)
        {
            if(IsKeyDown(binding.Key)){
                e.add(e);
            } else {
                e.remove(e);
            }
        });

    using PositiveBind = flecs::pair<KeyBinding, AxisBind::Positive>;
    using NegativeBind = flecs::pair<KeyBinding, AxisBind::Negative>;
    
    world.system<AxisValue, const PositiveBind, const NegativeBind>("Trigger Key Axis Binds")
        .kind(flecs::PreFrame)
        .each([](AxisValue& Axis, const KeyBinding& Positive, const KeyBinding& Negative)
        {
            Axis.Value = IsKeyDown(Positive.Key) ? 1.f : 0.f;
            Axis.Value -= IsKeyDown(Negative.Key) ? 1.f : 0.f;
        });
}
