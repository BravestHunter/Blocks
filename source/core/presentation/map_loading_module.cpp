#include "map_loading_module.hpp"

#include "environment.hpp"

#include "simulation/player_position_changed_event.hpp"
#include "simulation/chunk_updated_event.hpp"


namespace blocks
{
  MapLoadingModule::MapLoadingModule()
  {

  }

  MapLoadingModule::~MapLoadingModule()
  {

  }


  void MapLoadingModule::Update(float delta, PresentationContext& presentationContext, GameContext& gameContext)
  {
    std::lock_guard<std::mutex> locker(queueMutex_);

    while (!chunksActionQueue_.empty())
    {
      const ChunksQueueItem item = chunksActionQueue_.front();
      chunksActionQueue_.pop();
      
      if (item.add)
      {
        presentationContext.openglScene->GetMap()->AddChunk(item.chunkData, item.position);
      }
      else
      {
        presentationContext.openglScene->GetMap()->RemoveChunk(item.position);
      }
    }
  }

  void MapLoadingModule::ProcessModelUpdate(BaseModelUpdateEvent* e, PresentationContext& presentationContext, GameContext& gameContext)
  {
    if (gameContext.scene->ContainsWorld() == false)
    {
      return;
    }

    switch (e->GetType())
    {
      case ModelUpdateEventType::PlayerPositionChanged:
      {
        PlayerPositionChangedEvent* positionChangedEvent = static_cast<PlayerPositionChangedEvent*>(e);
        glm::ivec2 centerChunk = CalculateChunkCenter(positionChangedEvent->GetPosition());

        if (centerChunk != lastCenterChunkCoords_)
        {
          RemoveChunks(centerChunk, lastCenterChunkCoords_, presentationContext);
          AddChunks(centerChunk, gameContext.scene->GetWorld()->GetMap(), presentationContext);

          lastCenterChunkCoords_ = centerChunk;
        }
        break;
      }

      case ModelUpdateEventType::ChunkUpdated:
      {
        ChunkUpdatedEvent* positionChangedEvent = static_cast<ChunkUpdatedEvent*>(e);
        EnqueueChunkAdd(gameContext.scene->GetWorld()->GetMap(), positionChangedEvent->GetPosition(), presentationContext.blockSet);
        break;
      }
    }
  }


  void MapLoadingModule::OnSceneChanged(PresentationContext& presentationContext, GameContext& gameContext)
  {
    if (gameContext.scene->ContainsWorld())
    {
      lastCenterChunkCoords_ = CalculateChunkCenter(gameContext.camera->GetPosition());
      AddChunks(lastCenterChunkCoords_, gameContext.scene->GetWorld()->GetMap(), presentationContext);
    }
  }


  void MapLoadingModule::AddChunks(glm::ivec2 centerChunkCoords, std::shared_ptr<Map> map, PresentationContext& presentationContext)
  {
    std::shared_ptr<OpenglMap> openglMap = presentationContext.openglScene->GetMap();

    for (int x = centerChunkCoords.x - loadingRadius_; x <= centerChunkCoords.x + loadingRadius_; x++)
    {
      for (int y = centerChunkCoords.y - loadingRadius_; y <= centerChunkCoords.y + loadingRadius_; y++)
      {
        ChunkPosition position = { x, y };

        if (!openglMap->ContainsChunk(position))
        {
          EnqueueChunkAdd(map, position, presentationContext.blockSet);
        }
      }
    }
  }

  void MapLoadingModule::RemoveChunks(glm::ivec2 CenterChunkCoords, glm::ivec2 lastCenterChunkCoords, PresentationContext& presentationContext)
  {
    std::shared_ptr<OpenglMap> openglMap = presentationContext.openglScene->GetMap();
    glm::ivec2 xBorders = glm::ivec2(CenterChunkCoords.x - loadingRadius_, CenterChunkCoords.x + loadingRadius_);
    glm::ivec2 yBorders = glm::ivec2(CenterChunkCoords.y - loadingRadius_, CenterChunkCoords.y + loadingRadius_);

    for (int x = lastCenterChunkCoords.x - loadingRadius_; x <= lastCenterChunkCoords.x + loadingRadius_; x++)
    {
      for (int y = lastCenterChunkCoords.y - loadingRadius_; y <= lastCenterChunkCoords.y + loadingRadius_; y++)
      {
        if (x < xBorders.x || x > xBorders.y ||
            y < yBorders.x || y > yBorders.y)
        {
          ChunkPosition position = { x, y };
          EnqueueChunkRemove(position);
        }
      }
    }
  }

  glm::ivec2 MapLoadingModule::CalculateChunkCenter(glm::vec3 position)
  {
    return glm::ivec2((int)position.x / Chunk::Length, (int)position.y / Chunk::Width);
  }


  void MapLoadingModule::EnqueueChunkAdd(std::shared_ptr<Map> map, ChunkPosition position, std::shared_ptr<BlockSet> blockSet)
  {
    std::shared_ptr<Chunk> chunk = map->GetChunk(position);
    std::shared_ptr<Chunk> frontChunk = map->GetChunk({ position.first + 1, position.second });
    std::shared_ptr<Chunk> backChunk = map->GetChunk({ position.first - 1, position.second });
    std::shared_ptr<Chunk> rightChunk = map->GetChunk({ position.first, position.second + 1 });
    std::shared_ptr<Chunk> leftChunk = map->GetChunk({ position.first, position.second - 1 });

    std::vector<OpenglChunkVertex> rawData = GenerateRawChunkData(chunk, frontChunk, backChunk, rightChunk, leftChunk, blockSet);
    ChunksQueueItem item
    {
      .chunkData = rawData,
      .position = position,
      .add = true
    };

    std::lock_guard<std::mutex> locker(queueMutex_);
    chunksActionQueue_.push(item);
  }

  void MapLoadingModule::EnqueueChunkRemove(ChunkPosition position)
  {
    ChunksQueueItem item
    {
      .position = position,
      .add = false
    };

    std::lock_guard<std::mutex> locker(queueMutex_);
    chunksActionQueue_.push(item);
  }

  std::vector<OpenglChunkVertex> MapLoadingModule::GenerateRawChunkData(
    std::shared_ptr<Chunk> chunk,
    std::shared_ptr<Chunk> frontChunk,
    std::shared_ptr<Chunk> backChunk,
    std::shared_ptr<Chunk> rightChunk,
    std::shared_ptr<Chunk> leftChunk,
    std::shared_ptr<BlockSet> blockSet
  )
  {
    static const size_t VerticesPerBlockNumber = 4 * 6;
    static const size_t verticesPerChunkNumber = Chunk::BlocksNumber * VerticesPerBlockNumber;

    std::vector<OpenglChunkVertex> verticesData;
    verticesData.reserve(verticesPerChunkNumber);

    for (unsigned int z = 0; z < Chunk::Height; z++)
    {
      for (unsigned int y = 0; y < Chunk::Width; y++)
      {
        for (unsigned int x = 0; x < Chunk::Length; x++)
        {
          size_t blockIndex = x + y * Chunk::Width + z * Chunk::LayerBlocksNumber;

          if (chunk->blocks[blockIndex] == 0)
          {
            continue;
          }

          BlockInfo fBlock = blockSet->GetBlockInfo(chunk->blocks[blockIndex] - 1);

          glm::vec3 position(x, y, z);

          // Check front face
          bool isFrontBorder = x == Chunk::Length - 1;
          if ((isFrontBorder && frontChunk->blocks[blockIndex - Chunk::Length + 1] == 0) || (!isFrontBorder && chunk->blocks[blockIndex + 1] == 0))
          {
            // Add front face

            verticesData.push_back(packVertex(x, y, z, 0, 0, fBlock.textures[0]));
            verticesData.push_back(packVertex(x, y, z, 0, 1, fBlock.textures[0]));
            verticesData.push_back(packVertex(x, y, z, 0, 2, fBlock.textures[0]));
            verticesData.push_back(packVertex(x, y, z, 0, 2, fBlock.textures[0]));
            verticesData.push_back(packVertex(x, y, z, 0, 1, fBlock.textures[0]));
            verticesData.push_back(packVertex(x, y, z, 0, 3, fBlock.textures[0]));
          }

          // Check back face
          bool isBackBorder = x == 0;
          if ((isBackBorder && backChunk->blocks[blockIndex + Chunk::Length - 1] == 0) || (!isBackBorder && chunk->blocks[blockIndex - 1] == 0))
          {
            // Add back face

            verticesData.push_back(packVertex(x, y, z, 1, 0, fBlock.textures[1]));
            verticesData.push_back(packVertex(x, y, z, 1, 1, fBlock.textures[1]));
            verticesData.push_back(packVertex(x, y, z, 1, 2, fBlock.textures[1]));
            verticesData.push_back(packVertex(x, y, z, 1, 2, fBlock.textures[1]));
            verticesData.push_back(packVertex(x, y, z, 1, 1, fBlock.textures[1]));
            verticesData.push_back(packVertex(x, y, z, 1, 3, fBlock.textures[1]));
          }

          // Check right face
          bool isRightBorder = y == Chunk::Width - 1;
          if ((isRightBorder && rightChunk->blocks[blockIndex - Chunk::LayerBlocksNumber + Chunk::Length] == 0) || (!isRightBorder && chunk->blocks[blockIndex + Chunk::Length] == 0))
          {
            // Add right face

            verticesData.push_back(packVertex(x, y, z, 2, 0, fBlock.textures[2]));
            verticesData.push_back(packVertex(x, y, z, 2, 1, fBlock.textures[2]));
            verticesData.push_back(packVertex(x, y, z, 2, 2, fBlock.textures[2]));
            verticesData.push_back(packVertex(x, y, z, 2, 2, fBlock.textures[2]));
            verticesData.push_back(packVertex(x, y, z, 2, 1, fBlock.textures[2]));
            verticesData.push_back(packVertex(x, y, z, 2, 3, fBlock.textures[2]));
          }

          // Check left face
          bool isLeftBorder = y == 0;
          if ((isLeftBorder && leftChunk->blocks[blockIndex + Chunk::LayerBlocksNumber - Chunk::Length] == 0) || (!isLeftBorder && chunk->blocks[blockIndex - Chunk::Length] == 0))
          {
            // Add left face

            verticesData.push_back(packVertex(x, y, z, 3, 0, fBlock.textures[3]));
            verticesData.push_back(packVertex(x, y, z, 3, 1, fBlock.textures[3]));
            verticesData.push_back(packVertex(x, y, z, 3, 2, fBlock.textures[3]));
            verticesData.push_back(packVertex(x, y, z, 3, 2, fBlock.textures[3]));
            verticesData.push_back(packVertex(x, y, z, 3, 1, fBlock.textures[3]));
            verticesData.push_back(packVertex(x, y, z, 3, 3, fBlock.textures[3]));
          }

          // Check top face
          bool isTopBorder = z == Chunk::Height - 1;
          if (isTopBorder || chunk->blocks[blockIndex + Chunk::LayerBlocksNumber] == 0)
          {
            // Add top face

            verticesData.push_back(packVertex(x, y, z, 4, 0, fBlock.textures[4]));
            verticesData.push_back(packVertex(x, y, z, 4, 1, fBlock.textures[4]));
            verticesData.push_back(packVertex(x, y, z, 4, 2, fBlock.textures[4]));
            verticesData.push_back(packVertex(x, y, z, 4, 2, fBlock.textures[4]));
            verticesData.push_back(packVertex(x, y, z, 4, 1, fBlock.textures[4]));
            verticesData.push_back(packVertex(x, y, z, 4, 3, fBlock.textures[4]));
          }

          // Check bottom face
          bool isBottomBorder = z == 0;
          if (isBottomBorder || chunk->blocks[blockIndex - Chunk::LayerBlocksNumber] == 0)
          {
            // Add bottom face

            verticesData.push_back(packVertex(x, y, z, 5, 0, fBlock.textures[5]));
            verticesData.push_back(packVertex(x, y, z, 5, 1, fBlock.textures[5]));
            verticesData.push_back(packVertex(x, y, z, 5, 2, fBlock.textures[5]));
            verticesData.push_back(packVertex(x, y, z, 5, 2, fBlock.textures[5]));
            verticesData.push_back(packVertex(x, y, z, 5, 1, fBlock.textures[5]));
            verticesData.push_back(packVertex(x, y, z, 5, 3, fBlock.textures[5]));
          }
        }
      }
    }

    return verticesData;
  }
}
