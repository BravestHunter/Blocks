#include "opengl_map.hpp"

#include <algorithm>

#include "model/chunk.hpp"


OpenglMap::OpenglMap()
{

}

OpenglMap::~OpenglMap()
{

}


bool OpenglMap::ContainsChunk(std::pair<int, int> position)
{
  return chunks_.contains(position);
}

void OpenglMap::EnqueueChunkAdd(OpenglRawChunkData& chunkData, std::pair<int, int> position)
{
  ChunksQueueItem item(chunkData, position);

  mutex_.lock();
  addQueue_.push(item);
  mutex_.unlock();
}

void OpenglMap::EnqueueChunkRemove(std::pair<int, int> position)
{
  mutex_.lock();
  removeQueue_.push(position);
  mutex_.unlock();
}

void OpenglMap::ProcessQueues()
{
  mutex_.lock();

  while (!addQueue_.empty())
  {
    AddChunk(addQueue_.front());
    addQueue_.pop();
  }

  while (!removeQueue_.empty())
  {
    RemoveChunk(removeQueue_.front());
    removeQueue_.pop();
  }

  mutex_.unlock();
}


void OpenglMap::AddChunk(ChunksQueueItem& item)
{
  std::shared_ptr<OpenglBuffer> vbo = std::make_shared<OpenglBuffer>(GL_ARRAY_BUFFER);
  std::shared_ptr<OpenglVertexArrayObject> vao = std::make_shared<OpenglVertexArrayObject>();

  vao->Bind();
  vbo->Bind();
  vbo->SetData(sizeof(float) * item.chunkData.verticesDataLength, item.chunkData.verticesData);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  std::shared_ptr<OpenglChunk> chunk = std::make_shared<OpenglChunk>(vbo, vao, item.chunkData.verticesNumber);
  chunks_[item.position] = chunk;

  delete item.chunkData.verticesData;
}

void OpenglMap::RemoveChunk(std::pair<int, int> position)
{
  chunks_.erase(position);
}

