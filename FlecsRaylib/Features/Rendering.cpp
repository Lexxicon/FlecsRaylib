#include "Rendering.h"

#include "flecs.h"
#include "raylib.h"
#include "../Data/Visuals.h"

void Rendering::RegisterTypes(flecs::world& ecs)
{
    ecs.type().component<RenderPhases>()
        .add<RenderPhases::PreDraw>()
        .add<RenderPhases::Background>()
        .add<RenderPhases::Draw>()
        .add<RenderPhases::PostDraw>();
}

void Rendering::RegisterSystems(flecs::world& ecs)
{
    ecs.observer<WindowInfo>()
        .event(flecs::OnSet)
        .each([](WindowInfo& Info)
        {
            InitWindow(Info.Width, Info.Height, Info.Title.c_str());
        });
    ecs.system<const WindowInfo>("BeginDraw")
        .arg(1).singleton()
        .kind(flecs::type_id<RenderPhases::PreDraw>())
        .iter([](flecs::iter& Iter, const WindowInfo* windowInfo)
        {
            BeginDrawing();
            ClearBackground(windowInfo->ClearColor);
        });
    ecs.system("EndDraw")
        .kind(flecs::type_id<RenderPhases::PostDraw>())
        .iter([](flecs::iter& Iter)
        {       
            DrawFPS(10, 10);
            EndDrawing();
        });
    
    ecs.system<Transform, Circle>("Draw Circles")
        .kind(flecs::type_id<RenderPhases::Draw>())
        .each([](Transform& transform, Circle& circle)
        {
            DrawCircle(transform.translation.x, transform.translation.y, circle.Radius, circle.Color);
        });
}

void Rendering::InitGlobals(flecs::world& ecs)
{
    ecs.set<RenderPhases>({BuildRenderPipeline(ecs)});
    ecs.set<WindowInfo>({
        800,
        450,
        RAYWHITE,
        "raylib [core] example - basic window"
    });
    SetTargetFPS(60);
}

flecs::query<> Rendering::BuildRenderPipeline(flecs::world& ecs)

{
    auto builder = ecs.query_builder()
     .term(flecs::System)
     .order_by(0, [](
         ecs_entity_t e1, const void *ptr1, 
         ecs_entity_t e2, const void *ptr2)
         {
             return (e1 > e2) - (e1 < e2);
         })
     .group_by(flecs::type_id<RenderPhases>(), [](
         flecs::world_t* m_world,
         flecs::type_t m_table_type,
         flecs::entity_t m_grp_type,
         void*)
         {
             flecs::type TableType(m_world, m_table_type);
             for(auto ColId : TableType.vector())
             {
                 flecs::entity e(m_world, ColId);
                 if(e.is_valid() && e.has(flecs::ChildOf, m_grp_type))
                 {
                     int i = 0;
                     auto GrpType = flecs::entity(m_world, m_grp_type).get<flecs::Type>();
                     for(auto TypeID : flecs::type(m_world, GrpType->normalized).vector()){
                         if(ColId == TypeID)
                         {
                             return i;
                         }
                         i++;
                     }
                 }
             }
             ecs_os_err("Failed to find group for %s", TableType.str());
             return 0;
         });
    
    ecs.component<RenderPhases>()
        .children([&](flecs::entity Phase)
        {
            builder.term(Phase).oper(flecs::Or);
        });

    return builder.build();
}