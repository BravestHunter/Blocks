#pragma once

#include <map>
#include <queue>
#include <memory>
#include <mutex>

#include "opengl_chunk.hpp"
#include "opengl_raw_chunk_data.hpp"


class OpenglRenderSystem;

struct ChunksQueueItem
{
  OpenglRawChunkData chunkData;
  std::pair<int, int> position;
};

class OpenglMap
{
  friend OpenglRenderSystem;

public:
  OpenglMap();
  ~OpenglMap();

  bool ContainsChunk(std::pair<int, int> position);
  void EnqueueChunkAdd(OpenglRawChunkData& chunkData, std::pair<int, int> position);
  void EnqueueChunkRemove(std::pair<int, int> position);
  void ProcessQueues();

private:
  std::map<std::pair<int, int>, std::shared_ptr<OpenglChunk>> chunks_;
  std::queue<ChunksQueueItem> addQueue_;
  std::queue<std::pair<int, int>> removeQueue_;
  std::mutex mutex_;

  void AddChunk(ChunksQueueItem& item);
  void RemoveChunk(std::pair<int, int> position);
};

