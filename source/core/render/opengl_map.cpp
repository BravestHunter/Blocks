#include "opengl_map.hpp"

#include <algorithm>

#include <glm/glm.hpp>

#include "environment.hpp"
#include "chunk.hpp"
#include "resource/image.hpp"


namespace blocks
{
  OpenglMap::OpenglMap()
  {

  }

  OpenglMap::~OpenglMap()
  {

  }


  void OpenglMap::SetBlockSet(std::shared_ptr<BlockSet> blockSet)
  {
    blockSet_ = blockSet;

    ResourceBase& resourceBase = Environment::GetResource();

    std::vector<Image> images;
    for (int i = 0; i < blockSet->GetTexturesNumber(); i++)
    {
      images.push_back(resourceBase.ReadImage(blockSet->GetTexture(i)));
    }

    int resolution = blockSet->GetResolution();
    blocksTextureArray_ = std::make_shared<OpenglTexture2DArray>(images, resolution, resolution);
    blocksTextureArray_->Bind(0);
  }

  bool OpenglMap::HasBlockSet()
  {
    return blockSet_ != nullptr;
  }


  bool OpenglMap::ContainsChunk(ChunkPosition position)
  {
    return chunks_.contains(position);
  }

  void OpenglMap::EnqueueChunkAdd(std::shared_ptr<Map> map, ChunkPosition position)
  {
    std::shared_ptr<Chunk> chunk = map->GetChunk(position);
    std::shared_ptr<Chunk> frontChunk = map->GetChunk({ position.first + 1, position.second });
    std::shared_ptr<Chunk> backChunk = map->GetChunk({ position.first - 1, position.second });
    std::shared_ptr<Chunk> rightChunk = map->GetChunk({ position.first, position.second + 1 });
    std::shared_ptr<Chunk> leftChunk = map->GetChunk({ position.first, position.second - 1 });

    std::shared_ptr<OpenglRawChunkData> rawData = GenerateRawChunkData(chunk, frontChunk, backChunk, rightChunk, leftChunk);
    ChunksQueueItem item(rawData, position);

    std::lock_guard<std::mutex> locker(mutex_);
    addQueue_.push(item);
  }

  void OpenglMap::EnqueueChunkRemove(ChunkPosition position)
  {
    std::lock_guard<std::mutex> locker(mutex_);
    removeQueue_.push(position);
  }

  void OpenglMap::ProcessQueues()
  {
    std::lock_guard<std::mutex> locker(mutex_);

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
  }


  std::shared_ptr<OpenglRawChunkData> OpenglMap::GenerateRawChunkData(
    std::shared_ptr<Chunk> chunk,
    std::shared_ptr<Chunk> frontChunk,
    std::shared_ptr<Chunk> backChunk,
    std::shared_ptr<Chunk> rightChunk,
    std::shared_ptr<Chunk> leftChunk
  )
  {
    static const size_t VerticesPerBlockNumber = 4 * 6;
    static const size_t verticesPerChunkNumber = Chunk::BlocksNumber * VerticesPerBlockNumber;

    OpenglChunkVertex* verticesData = new OpenglChunkVertex[verticesPerChunkNumber];
    size_t verticesDataIndex = 0;
    size_t verticesNumber = 0;

    BlockInfo testBlock = blockSet_->GetBlockInfo(0);

    for (unsigned int z = 0; z < Chunk::Height; z++)
    {
      for (unsigned int y = 0; y < Chunk::Width; y++)
      {
        for (unsigned int x = 0; x < Chunk::Length; x++)
        {
          size_t blockIndex = x + y * Chunk::Width + z * Chunk::LayerBlocksNumber;

          if (chunk->blocks[blockIndex] == 0)
          {
            continue;
          }

          BlockInfo fBlock = blockSet_->GetBlockInfo(chunk->blocks[blockIndex] - 1);

          glm::vec3 position(x, y, z);

          // Check front face
          bool isFrontBorder = x == Chunk::Length - 1;
          if ((isFrontBorder && frontChunk->blocks[blockIndex - Chunk::Length + 1] == 0) || (!isFrontBorder && chunk->blocks[blockIndex + 1] == 0))
          {
            // Add front face

            verticesData[verticesDataIndex++] = packVertex(x, y, z, 0, 0, fBlock.textures[0]);
            verticesData[verticesDataIndex++] = packVertex(x, y, z, 0, 1, fBlock.textures[0]);
            verticesData[verticesDataIndex++] = packVertex(x, y, z, 0, 2, fBlock.textures[0]);
            verticesData[verticesDataIndex++] = packVertex(x, y, z, 0, 2, fBlock.textures[0]);
            verticesData[verticesDataIndex++] = packVertex(x, y, z, 0, 1, fBlock.textures[0]);
            verticesData[verticesDataIndex++] = packVertex(x, y, z, 0, 3, fBlock.textures[0]);

            verticesNumber += 6;
          }

          // Check back face
          bool isBackBorder = x == 0;
          if ((isBackBorder && backChunk->blocks[blockIndex + Chunk::Length - 1] == 0) || (!isBackBorder && chunk->blocks[blockIndex - 1] == 0))
          {
            // Add back face

            verticesData[verticesDataIndex++] = packVertex(x, y, z, 1, 0, fBlock.textures[1]);
            verticesData[verticesDataIndex++] = packVertex(x, y, z, 1, 1, fBlock.textures[1]);
            verticesData[verticesDataIndex++] = packVertex(x, y, z, 1, 2, fBlock.textures[1]);
            verticesData[verticesDataIndex++] = packVertex(x, y, z, 1, 2, fBlock.textures[1]);
            verticesData[verticesDataIndex++] = packVertex(x, y, z, 1, 1, fBlock.textures[1]);
            verticesData[verticesDataIndex++] = packVertex(x, y, z, 1, 3, fBlock.textures[1]);

            verticesNumber += 6;
          }

          // Check right face
          bool isRightBorder = y == Chunk::Width - 1;
          if ((isRightBorder && rightChunk->blocks[blockIndex - Chunk::LayerBlocksNumber + Chunk::Length] == 0) || (!isRightBorder && chunk->blocks[blockIndex + Chunk::Length] == 0))
          {
            // Add right face

            verticesData[verticesDataIndex++] = packVertex(x, y, z, 2, 0, fBlock.textures[2]);
            verticesData[verticesDataIndex++] = packVertex(x, y, z, 2, 1, fBlock.textures[2]);
            verticesData[verticesDataIndex++] = packVertex(x, y, z, 2, 2, fBlock.textures[2]);
            verticesData[verticesDataIndex++] = packVertex(x, y, z, 2, 2, fBlock.textures[2]);
            verticesData[verticesDataIndex++] = packVertex(x, y, z, 2, 1, fBlock.textures[2]);
            verticesData[verticesDataIndex++] = packVertex(x, y, z, 2, 3, fBlock.textures[2]);

            verticesNumber += 6;
          }

          // Check left face
          bool isLeftBorder = y == 0;
          if ((isLeftBorder && leftChunk->blocks[blockIndex + Chunk::LayerBlocksNumber - Chunk::Length] == 0) || (!isLeftBorder && chunk->blocks[blockIndex - Chunk::Length] == 0))
          {
            // Add left face

            verticesData[verticesDataIndex++] = packVertex(x, y, z, 3, 0, fBlock.textures[3]);
            verticesData[verticesDataIndex++] = packVertex(x, y, z, 3, 1, fBlock.textures[3]);
            verticesData[verticesDataIndex++] = packVertex(x, y, z, 3, 2, fBlock.textures[3]);
            verticesData[verticesDataIndex++] = packVertex(x, y, z, 3, 2, fBlock.textures[3]);
            verticesData[verticesDataIndex++] = packVertex(x, y, z, 3, 1, fBlock.textures[3]);
            verticesData[verticesDataIndex++] = packVertex(x, y, z, 3, 3, fBlock.textures[3]);

            verticesNumber += 6;
          }

          // Check top face
          bool isTopBorder = z == Chunk::Height - 1;
          if (isTopBorder || chunk->blocks[blockIndex + Chunk::LayerBlocksNumber] == 0)
          {
            // Add top face

            verticesData[verticesDataIndex++] = packVertex(x, y, z, 4, 0, fBlock.textures[4]);
            verticesData[verticesDataIndex++] = packVertex(x, y, z, 4, 1, fBlock.textures[4]);
            verticesData[verticesDataIndex++] = packVertex(x, y, z, 4, 2, fBlock.textures[4]);
            verticesData[verticesDataIndex++] = packVertex(x, y, z, 4, 2, fBlock.textures[4]);
            verticesData[verticesDataIndex++] = packVertex(x, y, z, 4, 1, fBlock.textures[4]);
            verticesData[verticesDataIndex++] = packVertex(x, y, z, 4, 3, fBlock.textures[4]);

            verticesNumber += 6;
          }

          // Check bottom face
          bool isBottomBorder = z == 0;
          if (isBottomBorder || chunk->blocks[blockIndex - Chunk::LayerBlocksNumber] == 0)
          {
            // Add bottom face

            verticesData[verticesDataIndex++] = packVertex(x, y, z, 5, 0, fBlock.textures[5]);
            verticesData[verticesDataIndex++] = packVertex(x, y, z, 5, 1, fBlock.textures[5]);
            verticesData[verticesDataIndex++] = packVertex(x, y, z, 5, 2, fBlock.textures[5]);
            verticesData[verticesDataIndex++] = packVertex(x, y, z, 5, 2, fBlock.textures[5]);
            verticesData[verticesDataIndex++] = packVertex(x, y, z, 5, 1, fBlock.textures[5]);
            verticesData[verticesDataIndex++] = packVertex(x, y, z, 5, 3, fBlock.textures[5]);

            verticesNumber += 6;
          }
        }
      }
    }

    return std::make_shared<OpenglRawChunkData>(verticesData, verticesNumber);
  }

  void OpenglMap::AddChunk(ChunksQueueItem& item)
  {
    std::shared_ptr<OpenglBuffer> vbo = std::make_shared<OpenglBuffer>(GL_ARRAY_BUFFER);
    std::shared_ptr<OpenglVertexArrayObject> vao = std::make_shared<OpenglVertexArrayObject>();

    vao->Bind();
    vbo->Bind();
    vbo->SetData(sizeof(OpenglChunkVertex) * item.chunkData->length, item.chunkData->data);

    glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, sizeof(OpenglChunkVertex), (void*)0);
    glEnableVertexAttribArray(0);

    std::shared_ptr<OpenglChunk> chunk = std::make_shared<OpenglChunk>(vbo, vao, item.chunkData->length);
    chunks_[item.position] = chunk;

    delete item.chunkData->data;
  }

  void OpenglMap::RemoveChunk(ChunkPosition position)
  {
    chunks_.erase(position);
  }
}
