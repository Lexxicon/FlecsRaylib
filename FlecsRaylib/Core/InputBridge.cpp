#include "InputBridge.h"

#include "Data/Inputs.h"


void InputBridge::RegisterTypes(flecs::world& ecs)
{
    ecs.component<MouseInfo>()
        .set<MouseInfo>({})
        .set<raylib::Vector2>({GetMousePosition()});

    ecs.entity<MoveVertical>()
        .add<AxisChord>()
        .set<KeyBinding, AxisChord::Positive>({KeyboardKey::KEY_S})
        .set<KeyBinding, AxisChord::Negative>({KeyboardKey::KEY_W});
    
    ecs.entity<MoveHorizontal>()
        .add<AxisChord>()
        .set<KeyBinding, AxisChord::Positive>({KeyboardKey::KEY_D})
        .set<KeyBinding, AxisChord::Negative>({KeyboardKey::KEY_A});
    
}

void InputBridge::RegisterSystems(flecs::world& ecs)
{
    ecs.system<MouseInfo, raylib::Vector2>("Update Mouse Position")
        .kind(flecs::PreFrame)
        .iter(UpdateMouse);
    
    ecs.system<const MouseBinding>("Trigger Mouse Bindings")
        .kind(flecs::PreFrame)
        .iter(TriggerMouseBindings);

    ecs.system<AxisChord, const KeyBinding, const KeyBinding>()
        .term_at(2).second<AxisChord::Positive>()
        .term_at(3).second<AxisChord::Negative>()
        .kind(flecs::PreFrame)
        .each([](AxisChord& Chord, const KeyBinding& Positive, const KeyBinding& Negative)
        {
            Chord.Value = IsKeyDown(Positive.Key) ? 1.f : 0.f;
            Chord.Value -= IsKeyDown(Negative.Key) ? 1.f : 0.f;
        });

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
