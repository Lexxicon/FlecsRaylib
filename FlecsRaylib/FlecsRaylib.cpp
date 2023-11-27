
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
    flecs::world ecs;
    
    ecs_log_set_level(1);
    ecs_log_enable_colors(false);

    std::vector<LifecycleHandle> Features{
        CoreRendering::MakeHandle(),
        InputBridge::MakeHandle(),
        UserInput::MakeHandle(),
        Rendering::MakeHandle(),
        WorldTimeHandler::MakeHandle()
    };
    LifecycleHandle::ProcessHandles(ecs, Features);
    
    ecs.component<MainWindow>()
        .add<Window>()
        .set<WindowSize>({{800, 450}})
        .set<WindowTitle>({"Basic Window"})
        .set<WindowFPS>({60});
    
    while (ecs.progress()) { }

    return 0;
}
