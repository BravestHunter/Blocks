#include "map.hpp"

#include <format>

#include "FastNoise/FastNoise.h"

#include "io/file_api.hpp"
#include "serialization.hpp"


namespace blocks
{
  const glm::vec3 halfVector(0.5f);
  const glm::vec3 oneVector(1.0f);


  Map::Map(MapData mapData, std::string path) : path_(path), seed_(mapData.seed)
  {
  }

  Map::~Map()
  {
    for (const auto chunkPair : chunks_)
    {
      SaveChunk(chunkPair.second, chunkPair.first);
    }
  }


  int Map::GetSeed()
  {
    return seed_;
  }

  std::shared_ptr<Chunk> Map::GetChunk(ChunkPosition position)
  {
    std::unique_lock<std::mutex> locker(mutex_);

    // Check if chunk is already loaded
    auto it = chunks_.find(position);
    if (it != chunks_.end())
    {
      return it->second;
    }
    
    locker.unlock();

    std::shared_ptr<Chunk> chunk;

    // check if chunk file exists
    std::string chunkFilePath = GetChunkFileName(path_, position);
    if (isPathExist(chunkFilePath))
    {
      chunk = LoadChunk(position);
    }
    else
    {
      chunk = GenerateChunk(position);
    }

    locker.lock();

    chunks_[position] = chunk;

    return chunk;
  }

  std::pair<std::map<ChunkPosition, std::shared_ptr<Chunk>>::iterator, std::map<ChunkPosition, std::shared_ptr<Chunk>>::iterator> Map::GetChunksIterator()
  {
    return std::make_pair(chunks_.begin(), chunks_.end());
  }

  void Map::SetChunk(ChunkPosition position, std::shared_ptr<Chunk> chunk)
  {
    std::lock_guard<std::mutex> locker(mutex_);

    chunks_[position] = chunk;
  }


  BlockLookAt Map::GetBlockLookAt(const blocks::Ray& ray)
  {
    ChunkPosition chunkPosition = CalculateChunkPosition(ray.origin);
    glm::vec3 localPosition = glm::vec3(ray.origin.x - chunkPosition.first * (int)Chunk::Length, ray.origin.y - chunkPosition.second * (int)Chunk::Width, ray.origin.z);
    if (localPosition.x < 0)
    {
      localPosition.x += Chunk::Length;
      chunkPosition.first--;
    }
    if (localPosition.y < 0)
    {
      localPosition.y += Chunk::Width;
      chunkPosition.second--;
    }

    std::shared_ptr<Chunk> chunk = GetChunk(chunkPosition);
    blocks::Ray localRay(localPosition, ray.direction);

    glm::ivec3 centralBlockPosition = glm::ivec3(localPosition);

    blocks::RayIntersectionPoint closestIntersectionPoint;
    glm::uvec3 intersectedBlock;
    int radius = 3;
    for (int x = centralBlockPosition.x - radius; x <= centralBlockPosition.x + radius; x++)
    {
      if (x < 0 || x >= Chunk::Length)
      {
        continue;
      }

      for (int y = centralBlockPosition.y - radius; y <= centralBlockPosition.y + radius; y++)
      {
        if (y < 0 || y >= Chunk::Width)
        {
          continue;
        }

        for (int z = centralBlockPosition.z - radius; z <= centralBlockPosition.z + radius; z++)
        {
          if (z < 0 || z >= Chunk::Height)
          {
            continue;
          }

          size_t blockIndex = Chunk::CalculateBlockIndex(static_cast<unsigned int>(x), static_cast<unsigned int>(y), static_cast<unsigned int>(z));
          if (chunk->blocks[blockIndex] == 0)
          {
            continue;
          }

          blocks::AABB blockBounds(glm::vec3(x, y, z) + halfVector, oneVector);
          blocks::RayIntersectionPoint intersectionPoint = CheckCollision(localRay, blockBounds);
          if (intersectionPoint.distance != FLT_MAX && closestIntersectionPoint.distance > intersectionPoint.distance)
          {
            closestIntersectionPoint = intersectionPoint;
            intersectedBlock = glm::uvec3(static_cast<unsigned int>(x), static_cast<unsigned int>(y), static_cast<unsigned int>(z));
          }
        }
      }
    }

    BlockLookAt result;
    result.chunkPosition = chunkPosition;
    result.blockPosition = intersectedBlock;
    if (closestIntersectionPoint.distance != FLT_MAX)
    {
      result.hit = true;
      if (closestIntersectionPoint.point.x == (float)intersectedBlock.x)
      {
        result.loockFromDirection = Direction::Back;
      }
      else if (closestIntersectionPoint.point.x == (float)(intersectedBlock.x + 1))
      {
        result.loockFromDirection = Direction::Forward;
      }
      else if (closestIntersectionPoint.point.y == (float)intersectedBlock.y)
      {
        result.loockFromDirection = Direction::Left;
      }
      else if (closestIntersectionPoint.point.y == (float)(intersectedBlock.y + 1))
      {
        result.loockFromDirection = Direction::Right;
      }
      else if (closestIntersectionPoint.point.z == (float)intersectedBlock.z)
      {
        result.loockFromDirection = Direction::Down;
      }
      else if (closestIntersectionPoint.point.z == (float)(intersectedBlock.z + 1))
      {
        result.loockFromDirection = Direction::Up;
      }
    }

    return result;
  }


  ChunkPosition Map::CalculateChunkPosition(glm::vec3 position)
  {
    return ChunkPosition(floor(position.x / Chunk::Length), floor(position.y / Chunk::Width));
  }


  std::shared_ptr<Chunk> Map::GenerateChunk(ChunkPosition position)
  {
    Chunk* chunk = new Chunk();

    float* highMap = new float[Chunk::LayerBlocksNumber];

    auto perlinNoise = FastNoise::New<FastNoise::Perlin>();
    auto minMax = perlinNoise->GenUniformGrid2D(highMap, position.first * Chunk::Length, position.second * Chunk::Width, Chunk::Length, Chunk::Width, 0.01f, seed_);

    Block blockType = (rand() % 4) + 1;
    for (unsigned int x = 0; x < Chunk::Length; x++)
    {
      for (unsigned int y = 0; y < Chunk::Width; y++)
      {
        float height = (highMap[x + y * Chunk::Length] + 1.0f) / 2.0f; // (0.0 - 1.0) range
        //int highBorder = (int)(height * Chunk::Height);
        int highBorder = (int)(height * Chunk::Height / 3.0f);

        for (unsigned int z = 0; z < highBorder; z++)
        {
          size_t blockIndex = Chunk::CalculateBlockIndex(x, y, z);
          chunk->blocks[blockIndex] = blockType;
        }
      }
    }

    return std::shared_ptr<Chunk>(chunk);
  }

  std::shared_ptr<Chunk> Map::LoadChunk(ChunkPosition position)
  {
    std::string chunkFilePath = GetChunkFileName(path_, position);
    std::vector<unsigned char> data = blocks::readBinaryFile(chunkFilePath);
    std::shared_ptr<Chunk> chunk(deserializeChunk(data));

    return chunk;
  }

  void Map::SaveChunk(std::shared_ptr<Chunk> chunk, ChunkPosition position)
  {
    if (!isPathExist(path_))
    {
      createDirectory(path_);
    }

    std::string chunkFilePath = GetChunkFileName(path_, position);

    std::vector<unsigned char> data = serializeChunk(chunk.get());
    blocks::saveBinaryFile(chunkFilePath, data);
  }


  std::string Map::GetChunkFileName(std::string mapPath, ChunkPosition position)
  {
    return std::format("{0}/{1}_{2}.chunk", mapPath, position.first, position.second);
  }
}
