#pragma once

#include <vector>

#include "chunk.hpp"
#include "world.hpp"


namespace blocks
{
  std::vector<unsigned char> serializeChunk(const Chunk* chunk);
  Chunk* deserializeChunk(const std::vector<unsigned char>& data);

  std::vector<unsigned char> serializeWorld(const World* world);
  World* deserializeWorld(const std::vector<unsigned char>& data);
}
