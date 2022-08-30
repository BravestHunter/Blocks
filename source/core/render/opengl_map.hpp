#pragma once

#include <vector>
#include <map>
#include <queue>
#include <memory>
#include <mutex>

#include "opengl_chunk.hpp"
#include "opengl_raw_chunk_data.hpp"
#include "render/opengl_texture_2d_array.hpp"
#include "chunk.hpp"
#include "scene/map.hpp"
#include "resource/block_set.hpp"


namespace blocks
{
  class OpenglRenderModule;

  struct ChunksQueueItem
  {
    std::vector<OpenglChunkVertex> chunkData;
    ChunkPosition position;
  };

  class OpenglMap
  {
    friend OpenglRenderModule;

  public:
    OpenglMap();
    ~OpenglMap();

    void SetBlockSet(std::shared_ptr<BlockSet> blockSet);
    bool HasBlockSet();

    bool ContainsChunk(ChunkPosition position);
    void EnqueueChunkAdd(std::shared_ptr<Map> map, ChunkPosition position);
    void EnqueueChunkRemove(ChunkPosition position);
    void ProcessQueues();

  private:
    std::map<ChunkPosition, std::shared_ptr<OpenglChunk>> chunks_;
    std::queue<ChunksQueueItem> addQueue_;
    std::queue<ChunkPosition> removeQueue_;
    std::mutex mutex_;
    std::shared_ptr<BlockSet> blockSet_;
    std::shared_ptr<OpenglTexture2DArray> blocksTextureArray_;

    std::vector<OpenglChunkVertex> GenerateRawChunkData(
      std::shared_ptr<Chunk> chunk, 
      std::shared_ptr<Chunk> frontChunk, 
      std::shared_ptr<Chunk> backChunk, 
      std::shared_ptr<Chunk> rightChunk, 
      std::shared_ptr<Chunk> leftChunk
    );
    void AddChunk(ChunksQueueItem& item);
    void RemoveChunk(ChunkPosition position);
  };
}
