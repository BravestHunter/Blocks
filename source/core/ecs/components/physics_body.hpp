#pragma once

#include "geometry/aabb.hpp"


namespace blocks
{
  struct PhysicsBody
  {
    AABB bounds;
    glm::vec3 velocity;
    bool isGrounded;
  };
}
