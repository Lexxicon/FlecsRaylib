#pragma once
#include "Vector2.hpp"

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
    KeyboardKey Bind;
};