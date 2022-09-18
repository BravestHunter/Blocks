#pragma once

#include <memory>
#include <unordered_set>

#include "chunk_position.hpp"
#include "task/task.hpp"


namespace blocks
{
  struct MapLoadingContext
  {
    std::unordered_set<ChunkPosition, ChunkPositionHasher> loadedChunks;
    std::unordered_map<ChunkPosition, std::shared_ptr<Task>, ChunkPositionHasher> chunkMeshingTasks;
  };
}
