#include "opengl_map_model.hpp"

#include "model/chunk.hpp"


OpenglMapMoodel::OpenglMapMoodel()
{

}

OpenglMapMoodel::~OpenglMapMoodel()
{

}


void OpenglMapMoodel::EnqueueChunk(OpenglRawChunkData& chunkData, std::pair<int, int> position)
{
  ChunksQueueItem item(chunkData, position);

  mutex_.lock();
  addQueue_.push(item);
  mutex_.unlock();
}

void OpenglMapMoodel::AddChunksFromQueue()
{
  mutex_.lock();

  while (!addQueue_.empty())
  {
    AddChunk(addQueue_.front());
    addQueue_.pop();
  }

  mutex_.unlock();
}


void OpenglMapMoodel::AddChunk(ChunksQueueItem& item)
{
  std::shared_ptr<OpenglBuffer> vbo = std::make_shared<OpenglBuffer>(GL_ARRAY_BUFFER);
  std::shared_ptr<OpenglVertexArrayObject> vao = std::make_shared<OpenglVertexArrayObject>();

  vao->Bind();
  vbo->Bind();
  vbo->SetData(sizeof(float) * item.chunkData.verticesDataLength, item.chunkData.verticesData);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  std::shared_ptr<OpenglChunkModel> chunk = std::make_shared<OpenglChunkModel>(vbo, vao, item.chunkData.verticesNumber);
  chunks_[item.position] = chunk;

  delete item.chunkData.verticesData;
}

