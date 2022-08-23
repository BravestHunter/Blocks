#include "serialization.hpp"

#include <cstring>


namespace blocks
{
  std::vector<unsigned char> serializeChunk(const Chunk* chunk)
  {
    std::vector<unsigned char> data(sizeof(Chunk));
    std::memcpy(&data[0], chunk, sizeof(Chunk));

    return data;
  }

  Chunk* deserializeChunk(const std::vector<unsigned char>& data)
  {
    Chunk* chunk = new Chunk();
    std::memcpy(chunk, &data[0], sizeof(Chunk));

    return chunk;
  }


  std::vector<unsigned char> serializeWorld(const WorldData* world)
  {
    std::vector<unsigned char> data(sizeof(WorldData));
    std::memcpy(&data[0], world, sizeof(WorldData));

    return data;
  }

  WorldData* deserializeWorld(const std::vector<unsigned char>& data)
  {
    WorldData* world = new WorldData();
    std::memcpy(world, &data[0], sizeof(WorldData));

    return world;
  }
}
