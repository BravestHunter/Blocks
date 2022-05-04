#pragma once

#include <map>
#include <queue>
#include <memory>
#include <mutex>

#include "opengl_chunk.hpp"
#include "opengl_raw_chunk_data.hpp"
#include "model/chunk.hpp"


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

  bool ContainsChunk(std::pair<int, int> position);
  void EnqueueChunkAdd(std::shared_ptr<Chunk> chunk, std::pair<int, int> position);
  void EnqueueChunkRemove(std::pair<int, int> position);
  void ProcessQueues();

private:
  std::map<std::pair<int, int>, std::shared_ptr<OpenglChunk>> chunks_;
  std::queue<ChunksQueueItem> addQueue_;
  std::queue<std::pair<int, int>> removeQueue_;
  std::mutex mutex_;

  std::shared_ptr<OpenglRawChunkData> GenerateRawChunkData(std::shared_ptr<Chunk> chunk);
  void AddChunk(ChunksQueueItem& item);
  void RemoveChunk(std::pair<int, int> position);
};

