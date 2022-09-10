#pragma once

#include <memory>
#include <array>
#include <vector>

#include "chunk_position.hpp"
#include "block_side.hpp"
#include "resource/block_set.hpp"
#include "scene/map.hpp"
#include "opengl_chunk_vertex.hpp"


namespace blocks
{
  class OpenglChunkBuilder
  {
  public:
    OpenglChunkBuilder(ChunkPosition position, std::shared_ptr<BlockSet> blockSet, std::shared_ptr<Map> map);

    std::vector<OpenglChunkVertex> GenerateRawChunkData();

  private:
    inline std::array<int, 4> GetAdjacentBlocks(glm::ivec3 position, BlockSide side);
    inline int GetBlock(glm::ivec3 position);

    ChunkPosition position_;
    std::shared_ptr<BlockSet> blockSet_;
    std::shared_ptr<Chunk> chunksGrid_[3][3];
  };
}
