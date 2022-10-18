#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>


namespace blocks
{
  struct AABB
  {
    AABB() : AABB(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)) {}
    AABB(glm::vec3 center, glm::vec3 size) : center(center), size(size), lowBorder(center - size * 0.5f), highBorder(center + size * 0.5f) {}
    AABB(const AABB& aabb) : center(aabb.center), size(aabb.size), lowBorder(aabb.lowBorder), highBorder(aabb.highBorder) {}

    void operator=(const AABB aabb)
    {
      glm::vec3& centerRef = const_cast<glm::vec3&>(center);
      glm::vec3& sizeRef = const_cast<glm::vec3&>(size);
      glm::vec3& lowBorderRef = const_cast<glm::vec3&>(lowBorder);
      glm::vec3& highBorderRef = const_cast<glm::vec3&>(highBorder);

      centerRef = aabb.center;
      sizeRef = aabb.size;
      lowBorderRef = aabb.lowBorder;
      highBorderRef = aabb.highBorder;
    }

    const glm::vec3 center;
    const glm::vec3 size;
    const glm::vec3 lowBorder;
    const glm::vec3 highBorder;
  };
}
