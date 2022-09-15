#include "map.hpp"

#include <format>

#include "FastNoise/FastNoise.h"

#include "io/file_api.hpp"
#include "serialization.hpp"


namespace blocks
{
  const glm::vec3 halfVector(0.5f);
  const glm::vec3 oneVector(1.0f);


  Map::Map(MapData mapData, std::string path) : path_(path), seed_(mapData.seed), mapGenerator_(seed_)
  {
  }

  Map::~Map()
  {
    for (const auto chunkPair : chunks_)
    {
      SaveChunk(chunkPair.second, chunkPair.first);
    }
  }


  ChunkPosition Map::CalculateChunkPosition(glm::vec3 position)
  {
    return ChunkPosition(floor(position.x / Chunk::Length), floor(position.y / Chunk::Width));
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
      Chunk* chunkPtr = mapGenerator_.GenerateChunk(position);
      chunk = std::shared_ptr<Chunk>(chunkPtr);
    }

    locker.lock();

    chunks_[position] = chunk;

    return chunk;
  }


  void Map::SetChunk(ChunkPosition position, std::shared_ptr<Chunk> chunk)
  {
    std::lock_guard<std::mutex> locker(mutex_);

    chunks_[position] = chunk;
  }


  MapRayCastResult Map::RayCast(const Ray& ray, float maxDistance)
  {
    ChunkPosition currentChunkPosition = CalculateChunkPosition(ray.origin);
    std::shared_ptr<Chunk> currentChunk = GetChunk(currentChunkPosition);

    glm::vec3 localRayOrigin = glm::vec3
    (
      ray.origin.x - currentChunkPosition.x * static_cast<int>(Chunk::Length), 
      ray.origin.y - currentChunkPosition.y * static_cast<int>(Chunk::Width), 
      ray.origin.z
    );
    glm::ivec3 currentBlockPosition(localRayOrigin);

    // Check start block
    if (currentBlockPosition.z >= 0 && currentBlockPosition.z < Chunk::Height)
    {
      size_t startBlockIndex = Chunk::CalculateBlockIndex(currentBlockPosition);
      if (currentChunk->blocks[startBlockIndex] != 0)
      {
        MapRayCastResult result
        {
          .hit = true,
          .chunkPosition = currentChunkPosition,
          .blockPosition = currentBlockPosition,
          .intersectionPoint = ray.origin,
          .intersectedSide = BlockSide::Unknown
        };

        return result;
      }
    }

    glm::vec3 sidesSquared(ray.direction.x * ray.direction.x, ray.direction.y * ray.direction.y, ray.direction.z * ray.direction.z);
    glm::vec3 unitStep
    (
      sqrt(1.0f + sidesSquared.y / sidesSquared.x + sidesSquared.z / sidesSquared.x),
      sqrt(1.0f + sidesSquared.x / sidesSquared.y + sidesSquared.z / sidesSquared.y),
      sqrt(1.0f + sidesSquared.x / sidesSquared.z + sidesSquared.y / sidesSquared.z)
    );
    glm::ivec3 step;
    glm::vec3 rayLength1D; // Lengths along different dimensions
    if (ray.direction.x < 0.0f)
    {
      step.x = -1;
      rayLength1D.x = (localRayOrigin.x - static_cast<float>(currentBlockPosition.x)) * unitStep.x;
    }
    else
    {
      step.x = 1;
      rayLength1D.x = (static_cast<float>(currentBlockPosition.x + 1) - localRayOrigin.x) * unitStep.x;
    }
    if (ray.direction.y < 0.0f)
    {
      step.y = -1;
      rayLength1D.y = (localRayOrigin.y - static_cast<float>(currentBlockPosition.y)) * unitStep.y;
    }
    else
    {
      step.y = 1;
      rayLength1D.y = (static_cast<float>(currentBlockPosition.y + 1) - localRayOrigin.y) * unitStep.y;
    }
    if (ray.direction.z < 0.0f)
    {
      step.z = -1;
      rayLength1D.z = (localRayOrigin.z - static_cast<float>(currentBlockPosition.z)) * unitStep.z;
    }
    else
    {
      step.z = 1;
      rayLength1D.z = (static_cast<float>(currentBlockPosition.z + 1) - localRayOrigin.z) * unitStep.z;
    }

    bool hit = false;
    float distance = 0.0f;
    while (hit == false && distance < maxDistance)
    {
      if (rayLength1D.x < rayLength1D.y && rayLength1D.x < rayLength1D.z)
      {
        currentBlockPosition.x += step.x;
        distance = rayLength1D.x;
        rayLength1D.x += unitStep.x;
      }
      else if (rayLength1D.y < rayLength1D.z)
      {
        currentBlockPosition.y += step.y;
        distance = rayLength1D.y;
        rayLength1D.y += unitStep.y;
      }
      else
      {
        currentBlockPosition.z += step.z;
        distance = rayLength1D.z;
        rayLength1D.z += unitStep.z;
      }

      ChunkPosition newChunkPosition = currentChunkPosition;
      if (currentBlockPosition.x < 0)
      {
        currentBlockPosition.x += Chunk::Length;
        newChunkPosition.x -= 1;
      }
      else if (currentBlockPosition.x >= Chunk::Length)
      {
        currentBlockPosition.x -= Chunk::Length;
        newChunkPosition.x += 1;
      }
      if (currentBlockPosition.y < 0)
      {
        currentBlockPosition.y += Chunk::Width;
        newChunkPosition.y -= 1;
      }
      else if (currentBlockPosition.y >= Chunk::Width)
      {
        currentBlockPosition.y -= Chunk::Width;
        newChunkPosition.y += 1;
      }

      if (currentChunkPosition != newChunkPosition)
      {
        currentChunkPosition = newChunkPosition;
        currentChunk = GetChunk(currentChunkPosition);
      }

      if (currentBlockPosition.z < 0 || currentBlockPosition.z >= Chunk::Height)
      {
        // Nothing to hit out of chunk bounds
        continue;
      }

      glm::uvec3 unsignedCurrentBlockPosition(currentBlockPosition);
      size_t blockIndex = Chunk::CalculateBlockIndex(unsignedCurrentBlockPosition);
      if (currentChunk->blocks[blockIndex] > 0)
      {
        hit = true;
      }
    }

    MapRayCastResult result;
    result.hit = hit;
    if (hit)
    {
      result.chunkPosition = currentChunkPosition;
      result.blockPosition = glm::uvec3(currentBlockPosition);
      result.intersectionPoint = ray.origin + ray.direction * distance;

      glm::vec3 localIntersectionPoint = glm::vec3
      (
        result.intersectionPoint.x - currentChunkPosition.x * static_cast<int>(Chunk::Length),
        result.intersectionPoint.y - currentChunkPosition.y * static_cast<int>(Chunk::Width),
        result.intersectionPoint.z
      );

      static const float threshold = 1e-4f;
      if (abs(localIntersectionPoint.x - static_cast<float>(result.blockPosition.x + 1)) < threshold)
      {
        result.intersectedSide = BlockSide::Front;
      }
      else if (abs(localIntersectionPoint.x - static_cast<float>(result.blockPosition.x)) < threshold)
      {
        result.intersectedSide = BlockSide::Back;
      }
      else if (abs(localIntersectionPoint.y - static_cast<float>(result.blockPosition.y + 1)) < threshold)
      {
        result.intersectedSide = BlockSide::Right;
      }
      else if (abs(localIntersectionPoint.y - static_cast<float>(result.blockPosition.y)) < threshold)
      {
        result.intersectedSide = BlockSide::Left;
      }
      else if (abs(localIntersectionPoint.z - static_cast<float>(result.blockPosition.z + 1)) < threshold)
      {
        result.intersectedSide = BlockSide::Top;
      }
      else
      {
        result.intersectedSide = BlockSide::Bottom;
      }
    }

    return result;
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
    return std::format("{0}/{1}_{2}.chunk", mapPath, position.x, position.y);
  }
}
