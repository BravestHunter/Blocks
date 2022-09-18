#include "map_loading_module.hpp"

#include "environment.hpp"

#include "simulation/player_position_changed_event.hpp"
#include "simulation/chunk_updated_event.hpp"
#include "render/opengl_chunk_vertex.hpp"
#include "render/opengl_chunk_builder.hpp"


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
      
      bool containsChunk = presentationContext.mapLoadingContext.loadedChunks.contains(item.position);
      presentationContext.mapLoadingContext.chunkMeshingTasks.erase(item.position);

      if (item.add && containsChunk)
      {
        presentationContext.openglScene->GetMap()->AddChunk(item.chunkData, item.position);
      }
      else if (item.add == false && containsChunk == false)
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
        ChunkPosition centerChunk = Map::CalculateChunkPosition(positionChangedEvent->GetPosition());

        if (centerChunk != lastCenterChunkPosition_)
        {
          RemoveChunks(centerChunk, lastCenterChunkPosition_, presentationContext);
          AddChunks(centerChunk, gameContext.scene->GetWorld()->GetMap(), presentationContext);

          lastCenterChunkPosition_ = centerChunk;
        }
        break;
      }

      case ModelUpdateEventType::ChunkUpdated:
      {
        ChunkUpdatedEvent* positionChangedEvent = static_cast<ChunkUpdatedEvent*>(e);
        EnqueueChunkAdd(gameContext.scene->GetWorld()->GetMap(), positionChangedEvent->GetPosition(), presentationContext);
        break;
      }
    }
  }


  void MapLoadingModule::OnSceneChanged(PresentationContext& presentationContext, GameContext& gameContext)
  {
    {
      std::unique_lock<std::mutex> locker(queueMutex_);

      presentationContext.mapLoadingContext.loadedChunks.clear();
      for (auto taskPair : presentationContext.mapLoadingContext.chunkMeshingTasks)
      {
        taskPair.second->Cancel();
      }
      presentationContext.mapLoadingContext.chunkMeshingTasks.clear();

      locker.unlock();
    }

    presentationContext.openglScene->GetMap()->Clear();

    if (gameContext.scene->ContainsWorld())
    {
      lastCenterChunkPosition_ = Map::CalculateChunkPosition(gameContext.camera->GetPosition());
      AddChunks(lastCenterChunkPosition_, gameContext.scene->GetWorld()->GetMap(), presentationContext);
    }
  }


  void MapLoadingModule::AddChunks(ChunkPosition centerChunkPosition, std::shared_ptr<Map> map, PresentationContext& presentationContext)
  {
    for (int x = centerChunkPosition.x - loadingRadius_; x <= centerChunkPosition.x + loadingRadius_; x++)
    {
      for (int y = centerChunkPosition.y - loadingRadius_; y <= centerChunkPosition.y + loadingRadius_; y++)
      {
        ChunkPosition position = { x, y };

        if (presentationContext.mapLoadingContext.loadedChunks.contains(position) == false)
        {
          EnqueueChunkAdd(map, position, presentationContext);
        }
      }
    }
  }

  void MapLoadingModule::RemoveChunks(ChunkPosition centerChunkPosition, ChunkPosition lastCenterChunkPosition, PresentationContext& presentationContext)
  {
    glm::ivec2 xBorders = glm::ivec2(centerChunkPosition.x - loadingRadius_, centerChunkPosition.x + loadingRadius_);
    glm::ivec2 yBorders = glm::ivec2(centerChunkPosition.y - loadingRadius_, centerChunkPosition.y + loadingRadius_);

    for (int x = lastCenterChunkPosition.x - loadingRadius_; x <= lastCenterChunkPosition.x + loadingRadius_; x++)
    {
      for (int y = lastCenterChunkPosition.y - loadingRadius_; y <= lastCenterChunkPosition.y + loadingRadius_; y++)
      {
        ChunkPosition position = { x, y };

        if (x < xBorders.x || x > xBorders.y ||
            y < yBorders.x || y > yBorders.y &&
            presentationContext.mapLoadingContext.loadedChunks.contains(position))
        {
          EnqueueChunkRemove(position, presentationContext);
        }
      }
    }
  }


  void MapLoadingModule::EnqueueChunkAdd(std::shared_ptr<Map> map, ChunkPosition position, PresentationContext& presentationContext)
  {
    std::shared_ptr<Task> task = std::make_shared<Task>(
      [this, map, position, presentationContext]()
      {
        std::vector<OpenglChunkVertex> rawData = OpenglChunkBuilder(position, presentationContext.blockSet, map).GenerateRawChunkData();
        ChunksQueueItem item
        {
          .chunkData = rawData,
          .position = position,
          .add = true
        };

        std::lock_guard<std::mutex> locker(queueMutex_);
        chunksActionQueue_.push(item);
      }
    );

    {
      std::unique_lock<std::mutex> locker(queueMutex_);

      presentationContext.mapLoadingContext.loadedChunks.insert(position);
      auto taskPair = presentationContext.mapLoadingContext.chunkMeshingTasks.find(position);
      if (taskPair != presentationContext.mapLoadingContext.chunkMeshingTasks.end())
      {
        taskPair->second->Cancel();
        taskPair->second = task;
      }
      else
      {
        presentationContext.mapLoadingContext.chunkMeshingTasks[position] = task;
      }

      locker.unlock();
    }

    Environment::GetTaskScheduler().EnqueueTask(task);
  }

  void MapLoadingModule::EnqueueChunkRemove(ChunkPosition position, PresentationContext& presentationContext)
  {
    ChunksQueueItem item
    {
      .position = position,
      .add = false
    };

    {
      std::lock_guard<std::mutex> locker(queueMutex_);

      chunksActionQueue_.push(item);
      presentationContext.mapLoadingContext.loadedChunks.erase(position);
      auto taskPair = presentationContext.mapLoadingContext.chunkMeshingTasks.find(position);
      if (taskPair != presentationContext.mapLoadingContext.chunkMeshingTasks.end())
      {
        taskPair->second->Cancel();
        presentationContext.mapLoadingContext.chunkMeshingTasks.erase(taskPair);
      }
    }
  }
}
