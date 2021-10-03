#pragma once
#include "../SubModules/raylib/src/raylib.h"

struct MouseInfo
{
    Vector2 Position;
    Vector2 PosDelta;
};

struct MouseButtons
{
    struct Left{};
    struct Middle{};
    struct Right{};
};