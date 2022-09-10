#pragma once

#include "chunk.hpp"
#include "chunk_position.hpp"


namespace blocks
{
  class MapGenerator
  {
  public:
    MapGenerator(int seed);

    Chunk* GenerateChunk(ChunkPosition position);

  private:
    int seed_;
  };
}
