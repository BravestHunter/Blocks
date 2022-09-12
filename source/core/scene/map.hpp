#pragma once

#include <string>
#include <utility>
#include <map>
#include <memory>
#include <mutex>

#include "map_ray_cast_result.hpp"
#include "geometry/collisions_api.hpp"
#include "chunk.hpp"
#include "chunk_position.hpp"
#include "map_data.hpp"
#include "map_generator.hpp"


namespace blocks
{
  class Map
  {
  public:
    Map(MapData mapData, std::string path);
    ~Map();

    static ChunkPosition CalculateChunkPosition(glm::vec3 position);

    inline int GetSeed() const;
    std::shared_ptr<Chunk> GetChunk(ChunkPosition position);
    std::pair<std::map<ChunkPosition, std::shared_ptr<Chunk>>::iterator, std::map<ChunkPosition, std::shared_ptr<Chunk>>::iterator> GetChunksIterator();

    void SetChunk(ChunkPosition position, std::shared_ptr<Chunk> chunk);

    MapRayCastResult RayCast(const Ray& ray, float maxDistance);

  private:
    std::shared_ptr<Chunk> LoadChunk(ChunkPosition position);
    void SaveChunk(std::shared_ptr<Chunk> chunk, ChunkPosition position);

    static inline std::string GetChunkFileName(std::string mapPath, ChunkPosition position);

    std::string path_;
    int seed_;
    std::map<ChunkPosition, std::shared_ptr<Chunk>> chunks_;
    std::mutex mutex_;
    MapGenerator mapGenerator_;
  };


  int Map::GetSeed() const
  {
    return seed_;
  }
}
