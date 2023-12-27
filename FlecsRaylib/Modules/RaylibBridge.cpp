#include "RaylibBridge.h"

#include "raylib.hpp"
#include "Vector2.hpp"
#include "Window.hpp"

RaylibBridge::RaylibBridge(flecs::world& world)
{
    flecs::enum_type<MouseButton>(world);
    flecs::enum_type<KeyboardKey>(world);
    flecs::enum_type<GamepadButton>(world);
    flecs::enum_type<GamepadAxis>(world);

    world.scope(0, [&]{
        world.component<raylib::Vector2>()
            .member("x", &raylib::Vector2::x)
            .member("y", &raylib::Vector2::y);

        world.component<raylib::Window>(nullptr, /*allow_tag*/ false)
            .opaque(world.component()
                    .member<raylib::Vector2>("Position")
                    )
            .serialize([](const flecs::serializer* s, const raylib::Window* data)
            {
                s->member("Position");
                s->value(data->GetPosition());
                return 0;
            })
            .ensure_member([](raylib::Window* dst, const char* member) -> void*
            {
                return nullptr;
            });
    });
}
