#pragma once

#include <utility>
#include <map>
#include <memory>

#include "model/chunk.hpp"


class Map
{
public:
  Map();
  ~Map();

  std::shared_ptr<Chunk> GetChunk(std::pair<int, int> coordinates);

private:
  std::map<std::pair<int, int>, std::shared_ptr<Chunk>> chunks_;
  int seed_;

  std::shared_ptr<Chunk> GenerateChunk(std::pair<int, int> coordinates);
};
