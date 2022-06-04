#pragma once

#include <glm/glm.hpp>


namespace blocks
{
  struct RayIntersectionPoint
  {
    glm::vec3 point;
    float distance = FLT_MAX;
  };
}
