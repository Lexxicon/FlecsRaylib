#pragma once
#include "Vector2.hpp"
#include "Window.hpp"
#include "flecs.h"

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

struct AxisBinding
{
    void* Positive = nullptr;
    void* Negative = nullptr;
};


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