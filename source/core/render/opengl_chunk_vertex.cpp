#include "opengl_chunk_vertex.hpp"

namespace blocks
{
  OpenglChunkVertex packVertex(unsigned int x, unsigned int y, unsigned int z, unsigned int face, unsigned int uv, unsigned int texture)
  {
    OpenglChunkVertex vertex;

    vertex.data = 0;
    vertex.data |= (x & 15);
    vertex.data |= (y & 15) << 4;
    vertex.data |= (z & 255) << 8;
    vertex.data |= (face & 7) << 16;
    vertex.data |= (uv & 3) << 19;
    vertex.data |= (texture & 2047) << 21;

    return vertex;
  }
}
