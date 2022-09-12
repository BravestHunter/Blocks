#pragma once

#include <utility>

#include <glm/glm.hpp>

#include "chunk_position.hpp"
#include "block_side.hpp"


namespace blocks
{
  struct BlockLookAt
  {
    bool hit = false;
    ChunkPosition chunkPosition;
    glm::uvec3 blockPosition;
    BlockSide intersectionSide;
  };
}
