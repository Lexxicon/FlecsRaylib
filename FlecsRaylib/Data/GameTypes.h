#pragma once

#include "Vector2.hpp"

struct ControlledCircle{};

struct TimedLife
{
    float StartTime;
    float TimeRemaining;
};

struct Position
{
    raylib::Vector2 Value;
};

struct Velocity
{
    raylib::Vector2 Value;
};

struct Drag
{
    raylib::Vector2 Value;
};