#include "map.hpp"

#include "FastNoise/FastNoise.h"

#include "io/file_api.hpp"


Map::Map()
{
  seed_ = rand();
}

Map::Map(int seed) : seed_(seed)
{

}

Map::~Map()
{

}


int Map::GetSeed()
{
  return seed_;
}

std::shared_ptr<Chunk> Map::GetChunk(std::pair<int, int> position)
{
  std::shared_ptr<Chunk> chunk;

  auto it = chunks_.find(position);
  if (it == chunks_.end())
  {
    chunk = GenerateChunk(position);
    chunks_.emplace(position, chunk);
  }
  else
  {
    chunk = it->second;
  }

  return chunk;
}

std::pair<std::map<std::pair<int, int>, std::shared_ptr<Chunk>>::iterator, std::map<std::pair<int, int>, std::shared_ptr<Chunk>>::iterator> Map::GetChunksIterator()
{
  return std::make_pair(chunks_.begin(), chunks_.end());
}

void Map::AddChunk(std::pair<int, int> position, std::shared_ptr<Chunk> chunk)
{
  chunks_[position] = chunk;
}


std::shared_ptr<Chunk> Map::GenerateChunk(std::pair<int, int> position)
{
  Chunk* chunk = new Chunk();

  float* highMap = new float[Chunk::LayerBlocksNumber];

  auto perlinNoise = FastNoise::New<FastNoise::Perlin>();
  auto minMax = perlinNoise->GenUniformGrid2D(highMap, position.first * Chunk::Length, position.second * Chunk::Width, Chunk::Length, Chunk::Width, 0.01f, seed_);

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
