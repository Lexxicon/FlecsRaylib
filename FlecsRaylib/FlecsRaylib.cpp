
#include <vector>

#include "Core/FeatureLifecycle.h"
#include "Data/Visuals.h"
#include "flecs.h"
#include "Features//Rendering.h"
#include "Features/UserInput.h"

int main(int argc, char* argv[])
{
    flecs::world ecs;

    std::vector<LifecycleHandle> Features{
        UserInput::MakeHandle(),
        Rendering::MakeHandle()
    };
    LifecycleHandle::ProcessHandles(ecs, Features);

    auto RenderRef = ecs.component<RenderPhases>().get_ref<RenderPhases>();
    while (ecs.progress())
    {
        RenderRef->Pipeline.each([](flecs::entity RenderSystem)
        {
            flecs::system<>(RenderSystem.world(), RenderSystem).run();
        });
    }

    return 0;
}
