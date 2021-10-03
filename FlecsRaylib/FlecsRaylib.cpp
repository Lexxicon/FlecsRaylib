
#include <vector>

#include "Core/FeatureLifecycle.h"
#include "Data/Visuals.h"
#include "SubModules/flecs/flecs.h"
#include "SubModules/raylib/src/raylib.h"
#include "Systems/Rendering.h"
#include "Systems/UserInput.h"

int main(int argc, char* argv[])
{
    
    // Initialization
    //--------------------------------------------------------------------------------------
    flecs::world* ecs = new flecs::world;

    std::vector<LifecycleHandle> Systems{
        UserInput::MakeHandle(),
        Rendering::MakeHandle()
    };
    LifecycleHandle::ProcessHandles(*ecs, Systems);
    
    //--------------------------------------------------------------------------------------

    auto RenderRef = ecs->component<RenderPhases>().get_ref<RenderPhases>();
    // Main game loop
    while (!WindowShouldClose() && ecs->progress())    // Detect window close button or ESC key
    {
        RenderRef->Pipeline.each([](flecs::entity RenderSystem)
        {
            flecs::system<>(RenderSystem.world(), RenderSystem).run();
        });
    }

    delete ecs;

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
