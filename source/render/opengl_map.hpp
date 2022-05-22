#pragma once

#include <map>
#include <queue>
#include <memory>
#include <mutex>

#include "opengl_chunk.hpp"
#include "opengl_raw_chunk_data.hpp"
#include "opengl_texture_2d_array.hpp"
#include "model/chunk.hpp"
#include "resource/block_set.hpp"


class OpenglRenderSystem;

struct ChunksQueueItem
{
  std::shared_ptr<OpenglRawChunkData> chunkData;
  std::pair<int, int> position;
};

class OpenglMap
{
  friend OpenglRenderSystem;

public:
  OpenglMap();
  ~OpenglMap();

  void SetBlockSet(std::shared_ptr<BlockSet> blockSet);
  bool HasBlockSet();

  bool ContainsChunk(std::pair<int, int> position);
  void EnqueueChunkAdd(std::shared_ptr<Chunk> chunk, std::pair<int, int> position);
  void EnqueueChunkRemove(std::pair<int, int> position);
  void ProcessQueues();

private:
  std::map<std::pair<int, int>, std::shared_ptr<OpenglChunk>> chunks_;
  std::queue<ChunksQueueItem> addQueue_;
  std::queue<std::pair<int, int>> removeQueue_;
  std::mutex mutex_;
  std::shared_ptr<BlockSet> blockSet_;
  std::shared_ptr<OpenglTexture2DArray> blocksTextureArray_;

  std::shared_ptr<OpenglRawChunkData> GenerateRawChunkData(std::shared_ptr<Chunk> chunk);
  void AddChunk(ChunksQueueItem& item);
  void RemoveChunk(std::pair<int, int> position);
};

