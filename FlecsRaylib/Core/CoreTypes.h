#pragma once
#include "Vector2.hpp"
#include "Window.hpp"
#include "flecs.h"
#include "Reflection.h"
#include "boost/pfr/core.hpp"
#include "boost/pfr/core_name.hpp"


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