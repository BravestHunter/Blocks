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


  bool OpenglMap::ContainsChunk(ChunkPosition position)
  {
    return chunks_.contains(position);
  }


  void OpenglMap::AddChunk(std::vector<OpenglChunkVertex> chunkData, ChunkPosition position)
  {
    std::shared_ptr<OpenglBuffer> vbo = std::make_shared<OpenglBuffer>(GL_ARRAY_BUFFER);
    std::shared_ptr<OpenglVertexArrayObject> vao = std::make_shared<OpenglVertexArrayObject>();

    vao->Bind();
    vbo->Bind();
    vbo->SetData(sizeof(OpenglChunkVertex) * chunkData.size(), chunkData.data());

    glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, sizeof(OpenglChunkVertex), (void*)0);
    glEnableVertexAttribArray(0);

    std::shared_ptr<OpenglChunk> chunk = std::make_shared<OpenglChunk>(vbo, vao, chunkData.size());
    chunks_[position] = chunk;
  }

  void OpenglMap::RemoveChunk(ChunkPosition position)
  {
    chunks_.erase(position);
  }
}
