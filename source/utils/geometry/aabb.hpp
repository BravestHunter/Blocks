#pragma once

#include <glm/glm.hpp>


namespace blocks
{
  struct AABB
  {
    glm::vec3 low;
    glm::vec3 high;
  };
}
