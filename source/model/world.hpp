#pragma once

#include <cstdint>

#include <glm/glm.hpp>


namespace blocks
{
  struct World
  {
    char name[32];
    std::int32_t seed;
    glm::vec3 playerPos;
  };
}
