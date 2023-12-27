
#include <vector>

#include "Core/FeatureLifecycle.h"
#include "flecs.h"
// #include "Core/CoreRendering.h"
// #include "Features/Rendering.h"
#include "Core/Reflection.h"
#include "Features/UserInput.h"
#include "Features/WorldTimeHandler.h"
#include "Modules/InputBindings.h"
#include "Modules/InputProcessing.h"
#include "Modules/Rendering.h"
#include "Modules/StdBridge.h"

struct SomeType
{
    std::vector<int> Values;
};

int main(int argc, char* argv[])
{
    ecs_os_init();
    flecs::world ecs;
    
    ecs_log_set_level(-1);
    ecs_log_enable_colors(false);

    ecs.import<StdBridge>();
    ecs.import<InputProcessing>();
    ecs.import<Rendering>();
    Reflection::Register<SomeType>(ecs);
    
    
    std::vector<LifecycleHandle> Features{
        // CoreRendering::MakeHandle(),
        UserInput::MakeHandle(),
        // Rendering::MakeHandle(),
        WorldTimeHandler::MakeHandle()
    };
    LifecycleHandle::ProcessHandles(ecs, Features);


    ecs.entity("Bob").set<SomeType>({{1, 2}});
    
    auto result = ecs.plecs_from_file("./Config/KeyBinds.plecs");

    // printf("Result: %d Type:%s %d\n",
    //     result,
    //     ecs.entity<MoveHorizontal>().type().str().c_str(),
    //     ecs.entity<MoveHorizontal>().get<Input::KeyBinding, Input::AxisChord::Positive>()->Key);
    
    ecs.component<Rendering::MainWindow>()
        .add<raylib::Window>()
        .set<Rendering::WindowSize>({{800, 450}})
        .set<Rendering::WindowTitle>({"Basic Window"})
        .set<Rendering::WindowFPS>({60});
    
    ecs.set<flecs::Rest>({});
    
    while (ecs.progress()) { }

    return 0;
}
