#include "WorldTimeHandler.h"

#include "Data/GameTypes.h"

void WorldTimeHandler::RegisterTypes(flecs::world& ecs)
{
}

void WorldTimeHandler::RegisterSystems(flecs::world& ecs)
{
    ecs.system<TimeLeftInPhase, WorldTime, const WorldTimeConfig>()
        .arg(1).second(flecs::Wildcard).inout(flecs::InOut)
        .arg(2).singleton()
        .arg(3).singleton()
        .write<DayPhase>()
        .write<NightPhase>()
        .kind(flecs::PreFrame)
        .iter(AdvanceDayPhase);
}

void WorldTimeHandler::InitGlobals(flecs::world& ecs)
{
    WorldTimeConfig TimeCfg;
    TimeCfg.DayDuration = 10;
    TimeCfg.NightDuration = 5;
    ecs.set<WorldTimeConfig>(TimeCfg);
    ecs.set<WorldTime>({1});

    ecs.component<WorldTime>()
        .add<DayPhase>()
        .set<TimeLeftInPhase,DayPhase>({TimeCfg.DayDuration});
}

void WorldTimeHandler::AdvanceDayPhase(flecs::iter& Iter, TimeLeftInPhase* Countdown, WorldTime* Time,
    const WorldTimeConfig* TimeConfig)
{
    for(auto i : Iter)
    {
        Countdown[i].Value -= Iter.delta_time();
        if(Countdown[i].Value <= 0)
        {
            auto e = Iter.entity(i);
            flecs::id_t CurrentPhase = Iter.id(1).second();
            flecs::id_t NextPhase;
            TimeLeftInPhase NextDuration{Countdown[i].Value};
            if(Iter.id(1).second() == flecs::type_id<DayPhase>())
            {
                NextPhase = flecs::type_id<NightPhase>();
                NextDuration.Value += TimeConfig->NightDuration;
            }else
            {
                NextPhase = flecs::type_id<DayPhase>();
                NextDuration.Value += TimeConfig->DayDuration;
                Time->CurrentDay++;
            }
            e.remove(CurrentPhase).remove<TimeLeftInPhase>(CurrentPhase);
            e.add(NextPhase).set<TimeLeftInPhase>(NextPhase, NextDuration);
        }
    } 
}
