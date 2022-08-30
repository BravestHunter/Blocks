#pragma once

#include <mutex>
#include <utility>
#include <deque>

#include "glm/glm.hpp"

#include "game_module_interface.hpp"
#include "scene/map.hpp"
#include "render/opengl_map.hpp"
#include "render/opengl_render_module.hpp"


namespace blocks
{
  class MapLoadingModule : public GameModuleInterface
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
    ~MapLoadingModule() override;

    void InitResources();

    virtual void Update(float delta, GameContext& context) override;
    void ProcessModelUpdate(BaseModelUpdateEvent* e, GameContext& context);

    void SetRenderModule(OpenglRenderModule* renderModule);
    void SetBlockSet(std::shared_ptr<BlockSet> blockSet);

    void OnSceneChanged(GameContext& context);

  private:
    void AddChunks(glm::ivec2 centerChunkCoords, std::shared_ptr<Map> map);
    void RemoveChunks(glm::ivec2 centerChunkCoords, glm::ivec2 lastCenterChunkCoords);
    inline glm::ivec2 CalculateChunkCenter(glm::vec3 position);

    OpenglRenderModule* renderModule_ = nullptr;
    std::shared_ptr<BlockSet> blockSet_;

    int loadingRadius_ = 3;
    glm::ivec2 lastCenterChunkCoords_;

    std::queue<ChunksQueueItem> chunksActionQueue_;
    std::mutex queueMutex_;

    void EnqueueChunkAdd(std::shared_ptr<Map> map, ChunkPosition position);
    void EnqueueChunkRemove(ChunkPosition position);
    std::vector<OpenglChunkVertex> GenerateRawChunkData(
      std::shared_ptr<Chunk> chunk,
      std::shared_ptr<Chunk> frontChunk,
      std::shared_ptr<Chunk> backChunk,
      std::shared_ptr<Chunk> rightChunk,
      std::shared_ptr<Chunk> leftChunk
    );
  };
}
