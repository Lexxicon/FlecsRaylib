#include "UserInput.h"

#include "../Data/Inputs.h"
#include "../Common.h"
#include "../Data/Visuals.h"

void UserInput::RegisterTypes(flecs::world& ecs)
{
    ecs.type("MouseButtons").component<MouseButtons>()
        .add<MouseButtons::Left>()
        .add<MouseButtons::Middle>()
        .add<MouseButtons::Right>();
}

void UserInput::RegisterSystems(flecs::world& ecs)
{
    auto UpdateBuilder = ecs.system<MouseInfo>("MouseUpdate");
    ecs.component<MouseButtons>().children([&](flecs::entity Button)
    {
        UpdateBuilder.term(Button).oper(flecs::Optional).inout(flecs::Out);
    });
    UpdateBuilder
        .kind(flecs::PreFrame)
        .iter(UpdateMouse);

    ecs.observer<MouseInfo>()
        .term<MouseButtons::Left>()
        .event(flecs::OnAdd)
        .iter([](flecs::iter& Iter, MouseInfo* Info)
        {
            if(Iter.event_id() == flecs::type_id<MouseButtons::Left>())
            {
                Iter.world().entity()
                    .set<Transform>({{Info->Position.x, Info->Position.y}})
                    .set<Circle>({GREEN, 10});
            }
        });
}

void UserInput::InitGlobals(flecs::world& ecs)
{
    ecs.entity("PlayerMouse").set<MouseInfo>({});
}

void UserInput::UpdateMouse(flecs::iter& Iter, MouseInfo* MouseInfos)
{
    auto MousePosition = GetMousePosition();
    for(auto i : Iter)
    {
        MouseInfos[i].PosDelta = MouseInfos[i].Position - MousePosition;
        MouseInfos[i].Position = MousePosition;
        auto e = Iter.entity(i);
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            e.add<MouseButtons::Left>();
        }else
        {
            e.remove<MouseButtons::Left>();
        }
        if(IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))
        {
            e.add<MouseButtons::Middle>();
        }else
        {
            e.remove<MouseButtons::Middle>();
        }
        if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            e.add<MouseButtons::Right>();
        }else
        {
            e.remove<MouseButtons::Right>();
        }
    }
}
