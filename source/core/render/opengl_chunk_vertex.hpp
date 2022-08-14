#pragma once

#include <cstdint>
#include <bit>


namespace blocks
{
  struct OpenglChunkVertex
  {
    static const std::int32_t XPositionMask = 0x0000000F;
    static const std::int32_t YPositionMask = 0x000000F0;
    static const std::int32_t ZPositionMask = 0x0000FF00;
    static const std::int32_t VertexIndexInBlockMask = 0x00070000;

    // 4 bits for block 'x' position in chunk, [0-15]
    // 4 bits for block 'y' position in chunk, [0-15]
    // 8 bits for block 'z' position in chunk, [0-255]
    // 3 bits for face, [0-5]
    // 2 bits for uv, [0-3]
    // 11 bits for texture, [0-2047]
    std::int32_t data;
  };

  OpenglChunkVertex packVertex(unsigned int x, unsigned int y, unsigned int z, unsigned int face, unsigned int uv, unsigned int texture);
}
