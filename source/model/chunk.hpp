#pragma once

#include <cstring>

#include <glm/glm.hpp>

#include "block.hpp"


namespace blocks
{
  struct Chunk
  {
    static const size_t Length = 16;
    static const size_t Width = 16;
    static const size_t Height = 256;
    static const size_t LayerBlocksNumber = Length * Width;
    static const size_t BlocksNumber = LayerBlocksNumber * Height;

    inline bool operator==(const Chunk& other) const
    {
      return std::memcmp(&blocks, &other.blocks, sizeof(Chunk)) == 0;
    }

    static inline size_t CalculateBlockIndex(glm::uvec3 position)
    {
      return CalculateBlockIndex(position.x, position.y, position.z);
    }

    static inline size_t CalculateBlockIndex(unsigned int x, unsigned int y, unsigned int z)
    {
      return x + y * Chunk::Length + z * Chunk::LayerBlocksNumber;
    }

    Block blocks[BlocksNumber];
  };
}
