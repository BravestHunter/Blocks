#pragma once

#include <cstring>

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

    Block blocks[BlocksNumber];
  };
}
