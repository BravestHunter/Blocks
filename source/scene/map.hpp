#pragma once

#include <utility>
#include <map>
#include <memory>

#include "model/chunk.hpp"


class Map
{
public:
  Map();
  Map(int seed);
  ~Map();

  int GetSeed();
  std::shared_ptr<Chunk> GetChunk(std::pair<int, int> position);
  std::pair<std::map<std::pair<int, int>, std::shared_ptr<Chunk>>::iterator, std::map<std::pair<int, int>, std::shared_ptr<Chunk>>::iterator> GetChunksIterator();

  void AddChunk(std::pair<int, int> position, std::shared_ptr<Chunk> chunk);

private:
  std::map<std::pair<int, int>, std::shared_ptr<Chunk>> chunks_;
  int seed_;

  std::shared_ptr<Chunk> GenerateChunk(std::pair<int, int> position);
};
