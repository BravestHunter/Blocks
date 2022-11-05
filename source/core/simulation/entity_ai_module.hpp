#pragma once

#include "game_context.hpp"
#include "time/time_state.hpp"
#include "input/input_state.hpp"


namespace blocks
{
  class EntityAiModule
  {
  public:
    EntityAiModule();

    void Update(const float delta, const TimeState& timeState, const InputState& inputState, GameContext& gameContext);

  private:
    glm::vec2 GetRandomDirection();
  };
}

