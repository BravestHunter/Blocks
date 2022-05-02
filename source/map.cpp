#include "map.hpp"


Map::Map()
{

}

Map::~Map()
{

}


std::shared_ptr<Chunk> Map::GetChunk(std::pair<int, int> coordinates)
{
  std::shared_ptr<Chunk> chunk;

  auto it = chunks_.find(coordinates);
  if (it == chunks_.end())
  {
    chunk = GenerateChunk();
    chunks_.emplace(coordinates, chunk);
  }
  else
  {
    chunk = it->second;
  }


  return GenerateChunk();
}


std::shared_ptr<Chunk> Map::GenerateChunk()
{
  Chunk* chunk = new Chunk();

  for (int z = 0; z < Chunk::Height; z++)
  {
    for (int y = 0; y < Chunk::Width; y++)
    {
      for (int x = 0; x < Chunk::Length; x++)
      {
        chunk->blocks[x + y * Chunk::Width + z * Chunk::LayerBlocksNumber] = rand() % 4;
      }
    }
  }

  return std::shared_ptr<Chunk>(chunk);
}
