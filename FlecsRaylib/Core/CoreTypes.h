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

struct AxisChord
{
    struct Positive{};
    struct Negative{};

    float Value = 0;
};


namespace RenderPhases
{
    struct PreDraw{};
    struct Background{};
    struct Draw{};
    struct PostDraw{};
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