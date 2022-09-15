#include "map_generator.hpp"

#include "FastNoise/FastNoise.h"


namespace blocks
{
  MapGenerator::MapGenerator(int seed) : seed_(seed)
  {
  }


  Chunk* MapGenerator::GenerateChunk(ChunkPosition position)
  {
    Chunk* chunk = new Chunk();

    float* highMap = new float[Chunk::LayerBlocksNumber];

    auto perlinNoise = FastNoise::New<FastNoise::Perlin>();
    auto fractalFB = FastNoise::New<FastNoise::FractalFBm>();
    fractalFB->SetSource(perlinNoise);
    fractalFB->SetGain(2.0f);
    fractalFB->SetWeightedStrength(2.0f);
    fractalFB->SetOctaveCount(2);
    fractalFB->SetLacunarity(0.5f);

    auto minMax = fractalFB->GenUniformGrid2D(highMap, position.x * Chunk::Length, position.y * Chunk::Width, Chunk::Length, Chunk::Width, 0.01f, seed_);

    Block blockType = (rand() % 4) + 1;
    for (unsigned int x = 0; x < Chunk::Length; x++)
    {
      for (unsigned int y = 0; y < Chunk::Width; y++)
      {
        float height = (highMap[x + y * Chunk::Length] + 1.0f) / 2.0f; // (0.0 - 1.0) range
        int highBorder = (int)(height * Chunk::Height);

        for (unsigned int z = 0; z < highBorder; z++)
        {
          size_t blockIndex = Chunk::CalculateBlockIndex(x, y, z);
          chunk->blocks[blockIndex] = blockType;
        }
      }
    }

    return chunk;
  }
}
