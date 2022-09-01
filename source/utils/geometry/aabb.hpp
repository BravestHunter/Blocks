#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>


namespace blocks
{
  struct AABB
  {
    AABB(glm::vec3 center, glm::vec3 size) : center(center), size(size), lowBorder(center - size * 0.5f), highBorder(center + size * 0.5f) { }

    const glm::vec3 center;
    const glm::vec3 size;
    const glm::vec3 lowBorder;
    const glm::vec3 highBorder;
  };
}
