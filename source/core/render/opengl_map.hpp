#pragma once

#include <vector>
#include <unordered_map>
#include <queue>
#include <memory>
#include <mutex>

#include "opengl_chunk_vertex.hpp"
#include "opengl_chunk.hpp"
#include "texture_2d_array.hpp"
#include "chunk.hpp"
#include "scene/map.hpp"
#include "resource/block_set.hpp"


namespace blocks
{
  class OpenglRenderModule;

  class OpenglMap
  {
    friend OpenglRenderModule;

  public:
    OpenglMap();
    ~OpenglMap();

    void SetBlockSet(std::shared_ptr<BlockSet> blockSet);

    bool ContainsChunk(ChunkPosition position);

    void AddChunk(std::vector<OpenglChunkVertex> chunkData, ChunkPosition position);
    void RemoveChunk(ChunkPosition position);
    void Clear();

  private:
    std::unordered_map<ChunkPosition, std::shared_ptr<OpenglChunk>, ChunkPositionHasher> chunks_;
    std::shared_ptr<opengl::Texture2DArray> blocksTextureArray_;
  };
}
