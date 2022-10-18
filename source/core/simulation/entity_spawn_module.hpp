#pragma once

#include "game_context.hpp"
#include "input/input_state.hpp"


namespace blocks
{
  class EntitySpawnModule
  {
  public:
    EntitySpawnModule();
    EntitySpawnModule(const EntitySpawnModule&) = delete;
    EntitySpawnModule(EntitySpawnModule&& other) = delete;
    EntitySpawnModule& operator=(const EntitySpawnModule&) = delete;
    EntitySpawnModule& operator=(EntitySpawnModule&& other) = delete;
    ~EntitySpawnModule();

    void Update(const float delta, const InputState& inputState, GameContext& gameContext);
  };
}
