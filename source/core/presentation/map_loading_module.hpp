#pragma once

#include <mutex>
#include <utility>
#include <array>
#include <vector>
#include <queue>

#include "glm/glm.hpp"

#include "block_side.hpp"
#include "scene/map.hpp"
#include "render/opengl_chunk_vertex.hpp"
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

    int loadingRadius_ = 6;
    ChunkPosition lastCenterChunkPosition_;

    std::queue<ChunksQueueItem> chunksActionQueue_;
    std::mutex queueMutex_;

    void EnqueueChunkAdd(std::shared_ptr<Map> map, ChunkPosition position, PresentationContext& presentationContext, bool enqueueTask = true);
    void EnqueueChunkRemove(ChunkPosition position, PresentationContext& presentationContext);
  };
}
