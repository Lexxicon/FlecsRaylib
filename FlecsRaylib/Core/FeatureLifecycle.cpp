#include "FeatureLifecycle.h"

void LifecycleHandle::ProcessHandles(flecs::world& ecs, std::vector<LifecycleHandle>& Handles)
{
    for(auto Hndl : Handles)
    {
        Hndl.RegisterTypes(ecs);
    }
    for(auto Hndl : Handles)
    {
        Hndl.RegisterSystems(ecs);
    }
    for(auto Hndl : Handles)
    {
        Hndl.InitGlobals(ecs);
    }
}
