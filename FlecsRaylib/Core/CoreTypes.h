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
    KeyboardKey Bind;
};


struct RenderPhases
{
    struct PreDraw{};
    struct Background{};
    struct Draw{};
    struct PostDraw{};

    RenderPhases() = default;
    ~RenderPhases()
    {
        printf("dtor %p\n", this);
    }

    explicit RenderPhases(const flecs::query<>& pipeline)
        : Pipeline(pipeline)
    {
        printf("ctor %p\n", this);
    }

    RenderPhases(const RenderPhases& other)
        : Pipeline(other.Pipeline)
    {
        printf("const move %p\n", this);
    }

    RenderPhases(RenderPhases&& other) noexcept
        : Pipeline(std::move(other.Pipeline))
    {
        printf("refref move %p\n", this);
    }

    RenderPhases& operator=(const RenderPhases& other)
    {
        printf("equals move %p\n", this);
        if (this == &other)
            return *this;
        Pipeline = other.Pipeline;
        return *this;
    }

    RenderPhases& operator=(RenderPhases&& other) noexcept
    {
        printf("equals refref move %p\n", this);
        if (this == &other)
            return *this;
        Pipeline = std::move(other.Pipeline);
        return *this;
    }

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