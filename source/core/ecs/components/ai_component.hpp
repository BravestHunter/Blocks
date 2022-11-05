#pragma once

#include <glm/glm.hpp>


namespace blocks
{
  enum class EntityAIBehavior
  {
    Idle,
    Wander
  };

  struct AiComponent
  {
    EntityAIBehavior behavior;
    float behaviorChangedTime;
  };

  struct WanderAiComponent
  {
    glm::vec2 movingDirection;
    glm::vec3 lastPosition;
  };
}
