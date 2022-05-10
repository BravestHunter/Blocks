#pragma once

#include <glm/glm.hpp>


struct RayIntersectionPoint
{
  glm::vec3 point;
  float distance = FLT_MAX;
};
