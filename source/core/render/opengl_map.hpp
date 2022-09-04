#pragma once

#include <vector>
#include <map>
#include <queue>
#include <memory>
#include <mutex>

#include "opengl_chunk.hpp"
#include "opengl_raw_chunk_data.hpp"
#include "render/opengl_texture_2d_array.hpp"
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
    std::map<ChunkPosition, std::shared_ptr<OpenglChunk>> chunks_;
    std::shared_ptr<OpenglTexture2DArray> blocksTextureArray_;
  };
}
