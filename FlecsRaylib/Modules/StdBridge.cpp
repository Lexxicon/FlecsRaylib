#include "StdBridge.h"

#include <string>
#include <vector>


StdBridge::StdBridge(flecs::world& world)
{
    world.component<std::string>()
        .opaque(flecs::String)
        .serialize([](const flecs::serializer *s, const std::string *data) {
            const char *str = data->c_str();
            return s->value(flecs::String, &str);
        })
        .assign_string([](std::string* data, const char *value) {
            *data = value;
        });
}
