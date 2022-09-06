#pragma once

#include <cstdint>
#include <bit>


namespace blocks
{
  struct OpenglChunkVertex
  {
    // 4 bits for block 'x' position in chunk, [0-15]
    // 4 bits for block 'y' position in chunk, [0-15]
    // 8 bits for block 'z' position in chunk, [0-255]
    // 3 bits for face, [0-5]
    // 2 bits for uv, [0-3]
    // 9 bits for texture, [0-511]
    // 2 bits for ao, [0-4]
    std::int32_t data;
  };

  OpenglChunkVertex packVertex(unsigned int x, unsigned int y, unsigned int z, unsigned int face, unsigned int uv, unsigned int texture, unsigned int ao);
}
