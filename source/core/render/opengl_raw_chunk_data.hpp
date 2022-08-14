#pragma once

#include "opengl_chunk_vertex.hpp"


namespace blocks
{
  struct OpenglRawChunkData
  {
    OpenglChunkVertex* data;
    size_t length;
  };
}
