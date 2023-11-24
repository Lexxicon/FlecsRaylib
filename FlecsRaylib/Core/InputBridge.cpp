#include "InputBridge.h"

void InputBridge::RegisterTypes(flecs::world& ecs)
{
}

void InputBridge::RegisterSystems(flecs::world& ecs)
{
    ecs.system<MouseInfo, raylib::Vector2>("Update Mouse Position")
        .kind(flecs::PreFrame)
        .iter(UpdateMouse);
    
    ecs.system<const MouseBinding>("Trigger Mouse Bindings")
        .kind(flecs::PreFrame)
        .iter(TriggerMouseBindings);

    ecs.system<const KeyBinding>("Trigger Key Bindings")
        .kind(flecs::PreFrame)
        .each([](flecs::entity e, const KeyBinding& binding){
            if(IsKeyDown(binding.Key)){
                e.add(e);
            } else {
                e.remove(e);
            }
        });
}

void InputBridge::InitGlobals(flecs::world& ecs)
{
    ecs.component<MouseInfo>()
        .set<MouseInfo>({})
        .set<raylib::Vector2>({GetMousePosition()});
}

void InputBridge::UpdateMouse(flecs::iter& Iter, MouseInfo* MouseInfos, raylib::Vector2* Positions)
{
    auto MousePosition = GetMousePosition();
    for(auto i : Iter)
    {
        MouseInfos[i].PosDelta = Positions[i] - MousePosition;
        Positions[i] = MousePosition;
    }
}

void InputBridge::TriggerMouseBindings(flecs::iter& Iter, const MouseBinding* binding)
{
    for(auto i : Iter)
    {
        auto e = Iter.entity(i);
        if(IsMouseButtonDown(binding[i].Bind))
        {
            e.add(e);
        }else
        {
            e.remove(e);
        }
    }
}

void InputBridge::TriggerKeyBindings(flecs::entity e, const KeyBinding& binding)
{
    if(IsKeyDown(binding.Key)){
        e.add(e);
    } else {
        e.remove(e);
    }
}
