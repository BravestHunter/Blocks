#pragma once

#include <utility>
#include <map>
#include <memory>
#include <mutex>

#include "model/chunk.hpp"
#include "collision/collisions_api.hpp"


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

  bool Collides(const AABB& bounds, glm::vec3 position);

private:
  std::map<std::pair<int, int>, std::shared_ptr<Chunk>> chunks_;
  int seed_;
  std::mutex mutex_;

  std::shared_ptr<Chunk> GenerateChunk(std::pair<int, int> position);
};
