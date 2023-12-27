#pragma once
#include "flecs.h"
#include "Window.hpp"

struct Rendering
{
    struct RenderPhases
    {
        struct PreDraw{};
        struct Background{};
        struct Draw{};
        struct PostDraw{};
    };

    struct MainWindow{};
    
    struct WindowSize
    {
        raylib::Vector2 dimension;
    };
    
    struct WindowTitle
    {
        std::string Title;
    };
    
    struct WindowFPS
    {
        int Target;
    };
    
    Rendering(flecs::world& world);
};
