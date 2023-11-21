
#include <vector>

#include "Core/FeatureLifecycle.h"
#include "Data/Visuals.h"
#include "flecs.h"
#include "Core/CoreRendering.h"
#include "Core/InputBridge.h"
#include "Features//Rendering.h"
#include "Features/UserInput.h"
#include "Features/WorldTimeHandler.h"

int main(int argc, char* argv[])
{
    ecs_os_init();

    struct QueryHolder
    {
        flecs::query<> Query;
    };
    struct SomeTag{};
    
    flecs::world ecs;
    
    flecs::query_builder<> build = ecs.query_builder<>().term<SomeTag>();
    // ecs.set<QueryHolder>({build.build()});
    //
    // ecs.entity().add<SomeTag>();
    // ecs.component<QueryHolder>().add<SomeTag>();
    //
    // ecs.get<QueryHolder>()->Query.each([](flecs::entity e)
    // {
    //     printf("found %llu\n", e.id());
    // });
    // return 0;

    std::vector<LifecycleHandle> Features{
        CoreRendering::MakeHandle(),
        InputBridge::MakeHandle(),
        UserInput::MakeHandle(),
        Rendering::MakeHandle(),
        WorldTimeHandler::MakeHandle()
    };
    LifecycleHandle::ProcessHandles(ecs, Features);
    
    ecs_log_set_level(1);
    ecs_log_enable_colors(false);

    flecs::query_builder<> RenderQueryBuilder = ecs.query_builder()
                                    .term(flecs::System)
                                    .term<RenderPhases::PreDraw>().oper(flecs::Or)
                                    .term<RenderPhases::Background>().oper(flecs::Or)
                                    .term<RenderPhases::Draw>().oper(flecs::Or)
                                    .term<RenderPhases::PostDraw>()
    ;

    ecs.set<RenderPhases>(RenderPhases{RenderQueryBuilder.build()});
    
    auto RenderRef = ecs.get_ref<RenderPhases>();
    while (ecs.progress())
    {
        RenderRef->Pipeline.each([](flecs::entity RenderSystem)
        {
            flecs::system(RenderSystem.world(), RenderSystem).run();
        });
    }

    return 0;
}
