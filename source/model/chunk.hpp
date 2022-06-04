#pragma once

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

    Block blocks[BlocksNumber];

    static bool AreEqual(const Chunk& chunk1, const Chunk& chunk2)
    {
      return memcmp(&chunk1.blocks, &chunk2.blocks, sizeof(Block) * BlocksNumber) == 0;
    }
  };
}
