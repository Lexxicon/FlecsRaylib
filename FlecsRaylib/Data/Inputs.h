#pragma once
#include "raylib.h"

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