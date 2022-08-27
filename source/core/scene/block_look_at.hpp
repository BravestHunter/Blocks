#pragma once

#include <utility>

#include <glm/glm.hpp>

#include "chunk_position.hpp"


namespace blocks
{
  enum class Direction
  {
    Forward,
    Back,
    Right,
    Left,
    Up,
    Down
  };

  struct BlockLookAt
  {
    bool hit = false;
    ChunkPosition chunkPosition;
    glm::ivec3 blockPosition;
    Direction loockFromDirection;
  };
}
