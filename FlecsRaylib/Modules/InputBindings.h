#pragma once

#include "flecs.h"
#include "raylib.h"
#include "Vector2.hpp"

struct Input
{

    struct AxisBind
    {
        struct Positive{};
        struct Negative{};
    };
    
    struct MouseInfo
    {
        raylib::Vector2 PosDelta;
    };

    struct MouseBinding
    {
        MouseButton Bind;
    };

    struct KeyBinding
    {
        KeyboardKey Key;
    };

    struct AxisValue
    {
        float Value = 0;
    };
    
    Input(flecs::world& world);
};
