#include "map.hpp"

#include "FastNoise/FastNoise.h"


Map::Map()
{
  seed_ = rand();
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
    chunk = GenerateChunk(coordinates);
    chunks_.emplace(coordinates, chunk);
  }
  else
  {
    chunk = it->second;
  }


  return chunk;
}


std::shared_ptr<Chunk> Map::GenerateChunk(std::pair<int, int> coordinates)
{
  Chunk* chunk = new Chunk();

  float* highMap = new float[Chunk::LayerBlocksNumber];

  auto perlinNoise = FastNoise::New<FastNoise::Perlin>();
  auto minMax = perlinNoise->GenUniformGrid2D(highMap, coordinates.first * Chunk::Length, coordinates.second * Chunk::Width, Chunk::Length, Chunk::Width, 0.01f, seed_);

  Block blockType = (rand() % 3) + 1;
  for (int x = 0; x < Chunk::Length; x++)
  {
    for (int y = 0; y < Chunk::Width; y++)
    {
      float height = (highMap[x + y * Chunk::Length] + 1.0f) / 2.0f; // (0.0 - 1.0) range
      int highBorder = (int)(height * Chunk::Height);

      for (int z = 0; z < highBorder; z++)
      {
        chunk->blocks[x + y * Chunk::Length + z * Chunk::LayerBlocksNumber] = blockType;
      }
    }
  }

  return std::shared_ptr<Chunk>(chunk);
}
