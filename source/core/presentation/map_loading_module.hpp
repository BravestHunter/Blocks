#pragma once

#include <mutex>
#include <utility>
#include <deque>

#include "glm/glm.hpp"

#include "game_module_interface.hpp"
#include "scene/map.hpp"
#include "render/opengl_map.hpp"
#include "render/opengl_render_module.hpp"
#include "presentation_context.hpp"


namespace blocks
{
  class MapLoadingModule
  {
    struct ChunksQueueItem
    {
      std::vector<OpenglChunkVertex> chunkData;
      ChunkPosition position;
      bool add;
    };

  public:
    MapLoadingModule();
    MapLoadingModule(const MapLoadingModule&) = delete;
    MapLoadingModule(MapLoadingModule&& other) = delete;
    MapLoadingModule& operator=(const MapLoadingModule&) = delete;
    MapLoadingModule& operator=(MapLoadingModule&& other) = delete;
    ~MapLoadingModule();

    void Update(float delta, PresentationContext& presentationContext, GameContext& gameContext);
    void ProcessModelUpdate(BaseModelUpdateEvent* e, PresentationContext& presentationContext, GameContext& gameContext);

    void OnSceneChanged(PresentationContext& presentationContext, GameContext& gameContext);

  private:
    void AddChunks(ChunkPosition centerChunkPosition, std::shared_ptr<Map> map, PresentationContext& presentationContext);
    void RemoveChunks(ChunkPosition centerChunkPosition, ChunkPosition lastCenterChunkPosition, PresentationContext& presentationContext);

    int loadingRadius_ = 3;
    ChunkPosition lastCenterChunkPosition_;

    std::queue<ChunksQueueItem> chunksActionQueue_;
    std::mutex queueMutex_;

    void EnqueueChunkAdd(std::shared_ptr<Map> map, ChunkPosition position, std::shared_ptr<BlockSet> blockSet);
    void EnqueueChunkRemove(ChunkPosition position);
    std::vector<OpenglChunkVertex> GenerateRawChunkData(
      std::shared_ptr<Chunk> chunk,
      std::shared_ptr<Chunk> frontChunk,
      std::shared_ptr<Chunk> backChunk,
      std::shared_ptr<Chunk> rightChunk,
      std::shared_ptr<Chunk> leftChunk,
      std::shared_ptr<BlockSet> blockSet
    );
  };
}
