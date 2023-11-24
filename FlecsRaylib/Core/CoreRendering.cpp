#include "CoreRendering.h"

void CoreRendering::RegisterTypes(flecs::world& ecs)
{
    ecs.component<RenderPhases>()
        .add<RenderPhases::PreDraw>()
        .add<RenderPhases::Background>()
        .add<RenderPhases::Draw>()
        .add<RenderPhases::PostDraw>();
}

void CoreRendering::RegisterSystems(flecs::world& ecs)
{
    ecs.observer<Window>("Window Lifecycle Manager")
        .event(flecs::OnAdd)
        .event(flecs::OnRemove)
        .iter(WindowLifecycleHandler);
    ecs.observer<Window, const WindowTitle>("Update Window Title")
        .event(flecs::OnSet)
        .iter(UpdateWindowTitle);
    ecs.observer<Window, WindowSize>("Update Window Size")
        .event(flecs::OnSet)
        .iter(UpdateWindowSize);
    ecs.observer<Window, WindowFPS>("Update Window FPS")
        .event(flecs::OnSet)
        .iter(UpdateWindowFPS);


    ecs.system<Window>("Check Exit Request")
        .kind(flecs::PostFrame)
        .iter([](flecs::iter& Iter, Window* Windows)
        {
            for(auto i : Iter)
            {
                if(Windows[i].hndl->ShouldClose())
                {
                    Iter.world().quit();
                }
            }
        });
    
    ecs.system<Window>("BeginDrawing")
        .kind<RenderPhases::PreDraw>()
        .iter(BeginDrawing);
    ecs.system<Window>("EndDraw")
        .kind<RenderPhases::PostDraw>()
        .iter(EndDrawing);
}

void CoreRendering::InitGlobals(flecs::world& ecs)
{
    ecs.set<RenderPhases>({BuildRenderPipeline(ecs)});
}

flecs::query<> CoreRendering::BuildRenderPipeline(flecs::world& ecs)
{
    auto builder = ecs.query_builder()
        .term(flecs::System)
        .order_by(0, CompareEntityID)
        .group_by(flecs::type_id<RenderPhases>(), GetTypeRank);
    
    ecs.component<RenderPhases>().children([&](flecs::entity Phase)
    {
        builder.term(Phase).or_();
    });
    
    // final term can't be an Or
    builder.oper(flecs::And);
    
    return builder.build();
}

int CoreRendering::CompareEntityID(ecs_entity_t e1, const void* ptr1, ecs_entity_t e2, const void* ptr2)
{
    return (e1 > e2) - (e1 < e2);
}

uint64_t CoreRendering::GetTypeRank(flecs::world_t* m_world, flecs::table_t* m_table_type, flecs::id_t m_grp_type, void*)
{
    flecs::table TableType(m_world, m_table_type);
    
    for(auto ColId : TableType.type())
    {
        flecs::entity e(m_world, ColId);
        if(e.is_valid() && e.has(flecs::ChildOf, m_grp_type))
        {
            int i = 0;
            const flecs::type GrpType = flecs::entity(m_world, m_grp_type).type();
            for(auto TypeID : GrpType){
                if(ColId == TypeID)
                {
                    return i;
                }
                i++;
            }
        }
    }
    return 0;
}

void CoreRendering::WindowLifecycleHandler(flecs::iter& Iter, Window* windows)
{
    if(Iter.event() == flecs::OnAdd)
    {
        for(auto i : Iter)
        {
            windows[i].hndl = new raylib::Window(100, 100);
        }
    }else if(Iter.event() == flecs::OnRemove)
    {
        for(auto i : Iter)
        {
            delete windows[i].hndl;
        }
    }
}

void CoreRendering::UpdateWindowTitle(flecs::iter& Iter, Window* Windows, const WindowTitle* Titles)
{
    if(Iter.event_id() == flecs::type_id<WindowTitle>())
    {
        for(auto i : Iter)
        {
            Windows[i].hndl->SetTitle(Titles[i].Title);
        }
    }
}

void CoreRendering::UpdateWindowSize(flecs::iter& Iter, Window* Windows, const WindowSize* Sizes)
{
    if(Iter.event_id() == flecs::type_id<WindowSize>())
    {
        for(auto i : Iter)
        {
            Windows[i].hndl->SetSize(Sizes[i].dimension);
        }
    }
}

void CoreRendering::UpdateWindowFPS(flecs::iter& Iter, Window* Windows, const WindowFPS* FPS)
{
    if(Iter.event_id() == flecs::type_id<WindowFPS>())
    {
        for(auto i : Iter)
        {
            Windows[i].hndl->SetTargetFPS(FPS[i].Target);
        }
    }
}

void CoreRendering::BeginDrawing(flecs::iter& Iter, Window* Windows)
{
    for(auto i : Iter)
    {
        Windows[i].hndl->BeginDrawing();
        Windows[i].hndl->ClearBackground(BLACK);
    }
}

void CoreRendering::EndDrawing(flecs::iter& Iter, Window* Windows)
{
    for(auto i : Iter)
    {
        DrawFPS(10, 10);
        Windows[i].hndl->EndDrawing();
    }
}