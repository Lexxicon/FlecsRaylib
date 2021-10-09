#pragma once

#include <string>

#include "Vector2.hpp"

struct ControlledCircle{};

struct Position
{
    raylib::Vector2 Value;
};

struct WorldTimeConfig
{
    float DayDuration;
    float NightDuration;
};

struct WorldTime
{
    int CurrentDay;
};

struct DayPhase{};
struct NightPhase{};
struct TimeLeftInPhase
{
    float Value;
};

struct Pawn
{
    std::string Name;
};

struct MaxHealth
{
    float Value;
};
struct Health
{
    float Current;
};

struct Job
{
    struct Miner{};
    struct LumberJack{};
    struct Builder{};
};