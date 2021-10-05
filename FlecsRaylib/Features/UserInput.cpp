#include "UserInput.h"

#include "Vector2.hpp"
#include "Data/Inputs.h"
#include "Data/Visuals.h"

void UserInput::RegisterTypes(flecs::world& ecs)
{
    ecs.type("MouseButtons").component<MouseButtons>()
        .add<MouseButtons::Left>()
        .add<MouseButtons::Middle>()
        .add<MouseButtons::Right>();

}

void UserInput::RegisterSystems(flecs::world& ecs)
{
    auto MouseUpdateBuilder = ecs.system<MouseInfo, raylib::Vector2>("MouseUpdate");
    auto ButtonType = ecs.component<MouseButtons>().get<flecs::Type>();
    for(auto ButtonID : flecs::type(ecs, ButtonType->normalized).vector()){
        MouseUpdateBuilder.term(ButtonID).oper(flecs::Optional).inout(flecs::Out);
    }
    MouseUpdateBuilder
        .kind(flecs::PreFrame)
        .iter(UpdateMouse);

    ecs.observer<raylib::Vector2>()
        .term<MouseButtons::Left>()
        .event(flecs::OnAdd)
        .iter([](flecs::iter& Iter, raylib::Vector2* Positions)
        {
            if(Iter.event_id() == flecs::type_id<MouseButtons::Left>())
            {
                for(auto i : Iter){
                    Iter.world().entity()
                        .set<raylib::Vector2>(Positions[i])
                        .set<Circle>({GREEN, 10});
                }
            }
        });
}

void UserInput::InitGlobals(flecs::world& ecs)
{
    ecs.component<MouseInfo>()
        .set<MouseInfo>({})
        .set<raylib::Vector2>({GetMousePosition()})
        .set<Circle>({WHITE, 5});
}

void UserInput::UpdateMouse(flecs::iter& Iter, MouseInfo* MouseInfos, raylib::Vector2* Positions)
{
    auto MousePosition = GetMousePosition();
    for(auto i : Iter)
    {
        MouseInfos[i].PosDelta = Positions[i] - MousePosition;
        Positions[i] = MousePosition;
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
