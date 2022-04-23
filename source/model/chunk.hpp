#pragma once

#include "block.hpp"


struct Chunk
{
  static const size_t Length = 16;
  static const size_t Width = 16;
  static const size_t Height = 256;
  static const size_t LayerBlocksNumber = Length * Width;
  static const size_t BlocksNumber = LayerBlocksNumber * Height;

  Block blocks[BlocksNumber];
};
