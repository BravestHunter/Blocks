#pragma once

#include <string>
#include <utility>
#include <map>
#include <memory>
#include <mutex>

#include "block_look_at.hpp"
#include "geometry/collisions_api.hpp"
#include "chunk.hpp"
#include "chunk_position.hpp"
#include "map_data.hpp"


namespace blocks
{
  class Map
  {
  public:
    Map(MapData mapData, std::string path);
    ~Map();

    int GetSeed();
    std::shared_ptr<Chunk> GetChunk(ChunkPosition position);
    std::pair<std::map<ChunkPosition, std::shared_ptr<Chunk>>::iterator, std::map<ChunkPosition, std::shared_ptr<Chunk>>::iterator> GetChunksIterator();

    void SetChunk(ChunkPosition position, std::shared_ptr<Chunk> chunk);

    bool Collides(const blocks::AABB& bounds, glm::vec3 position);
    BlockLookAt GetBlockLookAt(const blocks::Ray& ray);

  private:
    std::string path_;
    int seed_;
    std::map<ChunkPosition, std::shared_ptr<Chunk>> chunks_;
    std::mutex mutex_;

    std::shared_ptr<Chunk> GenerateChunk(ChunkPosition position);
    std::shared_ptr<Chunk> LoadChunk(ChunkPosition position);
    void SaveChunk(std::shared_ptr<Chunk> chunk, ChunkPosition position);

    static inline std::string GetChunkFileName(std::string mapPath, ChunkPosition position);
  };
}
