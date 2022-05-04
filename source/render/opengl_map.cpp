#include "opengl_map.hpp"

#include <algorithm>

#include <glm/glm.hpp>

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

void OpenglMap::EnqueueChunkAdd(std::shared_ptr<Chunk> chunk, std::pair<int, int> position)
{
  std::shared_ptr<OpenglRawChunkData> rawData = GenerateRawChunkData(chunk);
  ChunksQueueItem item(rawData, position);

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


struct Vertex
{
  float x;
  float y;
  float z;
  float texU;
  float texV;
  float texI;
};

void AddVertex(Vertex& vertex, float* data, size_t& index)
{
  data[index++] = vertex.x;
  data[index++] = vertex.y;
  data[index++] = vertex.z;
  data[index++] = vertex.texU;
  data[index++] = vertex.texV;
  data[index++] = vertex.texI;
}


std::shared_ptr<OpenglRawChunkData> OpenglMap::GenerateRawChunkData(std::shared_ptr<Chunk> chunk)
{
  static const size_t BlockVerticesNumber = 4 * 6;
  static const size_t VertexSize = sizeof(float) * 6;
  static const size_t verticesDataSize = Chunk::BlocksNumber * BlockVerticesNumber * VertexSize;

  float* verticesData = new float[verticesDataSize];
  size_t verticesDataIndex = 0;
  size_t verticesNumber = 0;
  for (int z = 0; z < Chunk::Height; z++)
  {
    for (int y = 0; y < Chunk::Width; y++)
    {
      for (int x = 0; x < Chunk::Length; x++)
      {
        size_t blockIndex = x + y * Chunk::Width + z * Chunk::LayerBlocksNumber;

        if (chunk->blocks[blockIndex] == 0)
        {
          continue;
        }

        float fBlock = (float)(chunk->blocks[blockIndex] - 1);

        glm::vec3 position(x, y, z);

        // Check forward face
        if (x == Chunk::Length - 1 || chunk->blocks[blockIndex + 1] == 0)
        {
          // Add forward face

          Vertex v1(x + 1, y + 1, z, 0.0f, 0.0f, fBlock);
          Vertex v2(x + 1, y, z, 1.0f, 0.0f, fBlock);
          Vertex v3(x + 1, y + 1, z + 1, 0.0f, 1.0f, fBlock);
          Vertex v4(x + 1, y, z + 1, 1.0f, 1.0f, fBlock);

          AddVertex(v1, verticesData, verticesDataIndex);
          AddVertex(v2, verticesData, verticesDataIndex);
          AddVertex(v3, verticesData, verticesDataIndex);
          AddVertex(v3, verticesData, verticesDataIndex);
          AddVertex(v2, verticesData, verticesDataIndex);
          AddVertex(v4, verticesData, verticesDataIndex);

          verticesNumber += 6;
        }

        // Check backward face
        if (x == 0 || chunk->blocks[blockIndex - 1] == 0)
        {
          // Add backward face

          Vertex v1(x, y, z, 0.0f, 0.0f, fBlock);
          Vertex v2(x, y + 1, z, 1.0f, 0.0f, fBlock);
          Vertex v3(x, y, z + 1, 0.0f, 1.0f, fBlock);
          Vertex v4(x, y + 1, z + 1, 1.0f, 1.0f, fBlock);

          AddVertex(v1, verticesData, verticesDataIndex);
          AddVertex(v2, verticesData, verticesDataIndex);
          AddVertex(v3, verticesData, verticesDataIndex);
          AddVertex(v3, verticesData, verticesDataIndex);
          AddVertex(v2, verticesData, verticesDataIndex);
          AddVertex(v4, verticesData, verticesDataIndex);

          verticesNumber += 6;
        }

        // Check right face
        if (y == Chunk::Width - 1 || chunk->blocks[blockIndex + Chunk::Length] == 0)
        {
          // Add right face

          Vertex v1(x, y + 1, z, 0.0f, 0.0f, fBlock);
          Vertex v2(x + 1, y + 1, z, 1.0f, 0.0f, fBlock);
          Vertex v3(x, y + 1, z + 1, 0.0f, 1.0f, fBlock);
          Vertex v4(x + 1, y + 1, z + 1, 1.0f, 1.0f, fBlock);

          AddVertex(v1, verticesData, verticesDataIndex);
          AddVertex(v2, verticesData, verticesDataIndex);
          AddVertex(v3, verticesData, verticesDataIndex);
          AddVertex(v3, verticesData, verticesDataIndex);
          AddVertex(v2, verticesData, verticesDataIndex);
          AddVertex(v4, verticesData, verticesDataIndex);

          verticesNumber += 6;
        }

        // Check left face
        if (y == 0 || chunk->blocks[blockIndex - Chunk::Length] == 0)
        {
          // Add left face

          Vertex v1(x + 1, y, z, 0.0f, 0.0f, fBlock);
          Vertex v2(x, y, z, 1.0f, 0.0f, fBlock);
          Vertex v3(x + 1, y, z + 1, 0.0f, 1.0f, fBlock);
          Vertex v4(x, y, z + 1, 1.0f, 1.0f, fBlock);

          AddVertex(v1, verticesData, verticesDataIndex);
          AddVertex(v2, verticesData, verticesDataIndex);
          AddVertex(v3, verticesData, verticesDataIndex);
          AddVertex(v3, verticesData, verticesDataIndex);
          AddVertex(v2, verticesData, verticesDataIndex);
          AddVertex(v4, verticesData, verticesDataIndex);

          verticesNumber += 6;
        }

        // Check upper face
        if (z == Chunk::Height - 1 || chunk->blocks[blockIndex + Chunk::LayerBlocksNumber] == 0)
        {
          // Add upper face

          Vertex v1(x + 1, y, z + 1, 0.0f, 0.0f, fBlock);
          Vertex v2(x, y, z + 1, 1.0f, 0.0f, fBlock);
          Vertex v3(x + 1, y + 1, z + 1, 0.0f, 1.0f, fBlock);
          Vertex v4(x, y + 1, z + 1, 1.0f, 1.0f, fBlock);

          AddVertex(v1, verticesData, verticesDataIndex);
          AddVertex(v2, verticesData, verticesDataIndex);
          AddVertex(v3, verticesData, verticesDataIndex);
          AddVertex(v3, verticesData, verticesDataIndex);
          AddVertex(v2, verticesData, verticesDataIndex);
          AddVertex(v4, verticesData, verticesDataIndex);

          verticesNumber += 6;
        }

        // Check bottom face
        if (z == 0 || chunk->blocks[blockIndex - Chunk::LayerBlocksNumber] == 0)
        {
          // Add bottom face

          Vertex v1(x, y, z, 0.0f, 0.0f, fBlock);
          Vertex v2(x + 1, y, z, 1.0f, 0.0f, fBlock);
          Vertex v3(x, y + 1, z, 0.0f, 1.0f, fBlock);
          Vertex v4(x + 1, y + 1, z, 1.0f, 1.0f, fBlock);

          AddVertex(v1, verticesData, verticesDataIndex);
          AddVertex(v2, verticesData, verticesDataIndex);
          AddVertex(v3, verticesData, verticesDataIndex);
          AddVertex(v3, verticesData, verticesDataIndex);
          AddVertex(v2, verticesData, verticesDataIndex);
          AddVertex(v4, verticesData, verticesDataIndex);

          verticesNumber += 6;
        }
      }
    }
  }

  return std::make_shared<OpenglRawChunkData>(verticesData, verticesDataIndex + 1, verticesNumber);
}

void OpenglMap::AddChunk(ChunksQueueItem& item)
{
  std::shared_ptr<OpenglBuffer> vbo = std::make_shared<OpenglBuffer>(GL_ARRAY_BUFFER);
  std::shared_ptr<OpenglVertexArrayObject> vao = std::make_shared<OpenglVertexArrayObject>();

  vao->Bind();
  vbo->Bind();
  vbo->SetData(sizeof(float) * item.chunkData->verticesDataLength, item.chunkData->verticesData);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  std::shared_ptr<OpenglChunk> chunk = std::make_shared<OpenglChunk>(vbo, vao, item.chunkData->verticesNumber);
  chunks_[item.position] = chunk;

  delete item.chunkData->verticesData;
}

void OpenglMap::RemoveChunk(std::pair<int, int> position)
{
  chunks_.erase(position);
}

