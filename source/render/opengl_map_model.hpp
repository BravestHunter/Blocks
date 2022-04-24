#pragma once

#include <map>
#include <queue>
#include <memory>
#include <mutex>

#include "opengl_chunk_model.hpp"
#include "opengl_raw_chunk_data.hpp"


struct ChunksQueueItem
{
  OpenglRawChunkData chunkData;
  std::pair<int, int> position;
};

class OpenglMapMoodel
{
public:
  OpenglMapMoodel();
  ~OpenglMapMoodel();

  void EnqueueChunk(OpenglRawChunkData& chunkData, std::pair<int, int> position);
  void AddChunksFromQueue();

  std::mutex mutex_;
  std::queue<ChunksQueueItem> addQueue_;
  std::map<std::pair<int, int>, std::shared_ptr<OpenglChunkModel>> chunks_;

private:
  void AddChunk(ChunksQueueItem& item);
};

