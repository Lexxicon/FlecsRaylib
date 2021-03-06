
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
    ecs_log_set_level(1);
    ecs_log_enable_colors(false);
    
    flecs::world ecs;

    std::vector<LifecycleHandle> Features{
        CoreRendering::MakeHandle(),
        InputBridge::MakeHandle(),
        UserInput::MakeHandle(),
        Rendering::MakeHandle(),
        WorldTimeHandler::MakeHandle()
    };
    LifecycleHandle::ProcessHandles(ecs, Features);

    auto RenderRef = ecs.component<RenderPhases>().get_ref<RenderPhases>();
    while (ecs.progress())
    {
        RenderRef->Pipeline.each([](flecs::entity RenderSystem)
        {
            flecs::system(RenderSystem.world(), RenderSystem).run();
        });
    }

    return 0;
}
