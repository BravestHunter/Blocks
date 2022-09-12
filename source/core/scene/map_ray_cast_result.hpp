#pragma once

#include <glm/glm.hpp>

#include "chunk_position.hpp"
#include "block_side.hpp"


namespace blocks
{
  struct MapRayCastResult
  {
    bool hit;
    ChunkPosition chunkPosition;
    glm::uvec3 blockPosition;
    glm::vec3 intersectionPoint;
    BlockSide intersectedSide;
  };
}
