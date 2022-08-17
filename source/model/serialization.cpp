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


  std::vector<unsigned char> serializeWorld(const World* world)
  {
    std::vector<unsigned char> data(sizeof(World));
    std::memcpy(&data[0], world, sizeof(World));

    return data;
  }

  World* deserializeWorld(const std::vector<unsigned char>& data)
  {
    World* world = new World();
    std::memcpy(world, &data[0], sizeof(World));

    return world;
  }
}
