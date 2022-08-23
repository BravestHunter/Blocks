#pragma once

#include <vector>

#include "chunk.hpp"
#include "world_data.hpp"


namespace blocks
{
  std::vector<unsigned char> serializeChunk(const Chunk* chunk);
  Chunk* deserializeChunk(const std::vector<unsigned char>& data);

  std::vector<unsigned char> serializeWorld(const WorldData* world);
  WorldData* deserializeWorld(const std::vector<unsigned char>& data);
}
