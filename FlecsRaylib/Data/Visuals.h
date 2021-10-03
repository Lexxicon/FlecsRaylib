#pragma once

#include <string>

#include "flecs.h"
#include "raylib.h"
#include "Vector2.hpp"
#include "Window.hpp"

struct RenderPhases
{
    struct PreDraw{};
    struct Background{};
    struct Draw{};
    struct PostDraw{};

    flecs::query<> Pipeline;
};

struct MainWindow{};
struct Window
{
    raylib::Window* hndl;
};
struct WindowSize
{
    raylib::Vector2 dimension;
};
struct WindowTitle{
    std::string Title;
};
struct WindowFPS
{
    int Target;
};

struct Circle
{
    Color Color;
    float Radius;
};