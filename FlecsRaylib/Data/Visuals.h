#pragma once

#include <string>

#include "../SubModules/flecs/flecs.h"
#include "../SubModules/raylib/src/raylib.h"

struct RenderPhases
{
    struct PreDraw{};
    struct Background{};
    struct Draw{};
    struct PostDraw{};

    flecs::query<> Pipeline;
};

struct WindowInfo
{
    int Width;
    int Height;
    Color ClearColor;
    std::string Title;
};

struct Circle
{
    Color Color;
    float Radius;
};