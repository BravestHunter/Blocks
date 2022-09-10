#include "opengl_chunk_builder.hpp"


namespace blocks
{
  OpenglChunkBuilder::OpenglChunkBuilder(ChunkPosition position, std::shared_ptr<BlockSet> blockSet, std::shared_ptr<Map> map)
    : position_(position), blockSet_(blockSet), chunksGrid_
        {
          { map->GetChunk({ position.first - 1, position.second - 1 }), map->GetChunk({ position.first - 1, position.second }), map->GetChunk({ position.first - 1, position.second + 1 }) },
          { map->GetChunk({ position.first, position.second - 1 }), map->GetChunk(position), map->GetChunk({ position.first, position.second + 1 }) },
          { map->GetChunk({ position.first + 1, position.second - 1 }), map->GetChunk({ position.first + 1, position.second }), map->GetChunk({ position.first + 1, position.second + 1 }) }
        }

  {
  }


  std::vector<OpenglChunkVertex> OpenglChunkBuilder::GenerateRawChunkData()
  {
    static const size_t VerticesPerBlockNumber = 4 * 6;
    static const size_t verticesPerChunkNumber = Chunk::BlocksNumber * VerticesPerBlockNumber;

    std::shared_ptr<Chunk> chunk = chunksGrid_[1][1];
    std::shared_ptr<Chunk> frontChunk = chunksGrid_[2][1];
    std::shared_ptr<Chunk> backChunk = chunksGrid_[0][1];
    std::shared_ptr<Chunk> rightChunk = chunksGrid_[1][2];
    std::shared_ptr<Chunk> leftChunk = chunksGrid_[1][0];

    std::vector<OpenglChunkVertex> verticesData;
    verticesData.reserve(verticesPerChunkNumber);

    for (unsigned int z = 0; z < Chunk::Height; z++)
    {
      for (unsigned int y = 0; y < Chunk::Width; y++)
      {
        for (unsigned int x = 0; x < Chunk::Length; x++)
        {
          size_t blockIndex = Chunk::CalculateBlockIndex(x, y, z);

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

            std::array<int, 4> adjacentBlocks = GetAdjacentBlocks(position, BlockSide::Front);

            verticesData.push_back(packVertex(x, y, z, 0, 0, fBlock.textures[0], adjacentBlocks[0]));
            verticesData.push_back(packVertex(x, y, z, 0, 1, fBlock.textures[0], adjacentBlocks[1]));
            verticesData.push_back(packVertex(x, y, z, 0, 2, fBlock.textures[0], adjacentBlocks[2]));
            verticesData.push_back(packVertex(x, y, z, 0, 2, fBlock.textures[0], adjacentBlocks[2]));
            verticesData.push_back(packVertex(x, y, z, 0, 1, fBlock.textures[0], adjacentBlocks[1]));
            verticesData.push_back(packVertex(x, y, z, 0, 3, fBlock.textures[0], adjacentBlocks[3]));
          }

          // Check back face
          bool isBackBorder = x == 0;
          if ((isBackBorder && backChunk->blocks[blockIndex + Chunk::Length - 1] == 0) || (!isBackBorder && chunk->blocks[blockIndex - 1] == 0))
          {
            // Add back face

            std::array<int, 4> adjacentBlocks = GetAdjacentBlocks(position, BlockSide::Back);

            verticesData.push_back(packVertex(x, y, z, 1, 0, fBlock.textures[1], adjacentBlocks[0]));
            verticesData.push_back(packVertex(x, y, z, 1, 1, fBlock.textures[1], adjacentBlocks[1]));
            verticesData.push_back(packVertex(x, y, z, 1, 2, fBlock.textures[1], adjacentBlocks[2]));
            verticesData.push_back(packVertex(x, y, z, 1, 2, fBlock.textures[1], adjacentBlocks[2]));
            verticesData.push_back(packVertex(x, y, z, 1, 1, fBlock.textures[1], adjacentBlocks[1]));
            verticesData.push_back(packVertex(x, y, z, 1, 3, fBlock.textures[1], adjacentBlocks[3]));
          }

          // Check right face
          bool isRightBorder = y == Chunk::Width - 1;
          if ((isRightBorder && rightChunk->blocks[blockIndex - Chunk::LayerBlocksNumber + Chunk::Length] == 0) || (!isRightBorder && chunk->blocks[blockIndex + Chunk::Length] == 0))
          {
            // Add right face

            std::array<int, 4> adjacentBlocks = GetAdjacentBlocks(position, BlockSide::Right);

            verticesData.push_back(packVertex(x, y, z, 2, 0, fBlock.textures[2], adjacentBlocks[0]));
            verticesData.push_back(packVertex(x, y, z, 2, 1, fBlock.textures[2], adjacentBlocks[1]));
            verticesData.push_back(packVertex(x, y, z, 2, 2, fBlock.textures[2], adjacentBlocks[2]));
            verticesData.push_back(packVertex(x, y, z, 2, 2, fBlock.textures[2], adjacentBlocks[2]));
            verticesData.push_back(packVertex(x, y, z, 2, 1, fBlock.textures[2], adjacentBlocks[1]));
            verticesData.push_back(packVertex(x, y, z, 2, 3, fBlock.textures[2], adjacentBlocks[3]));
          }

          // Check left face
          bool isLeftBorder = y == 0;
          if ((isLeftBorder && leftChunk->blocks[blockIndex + Chunk::LayerBlocksNumber - Chunk::Length] == 0) || (!isLeftBorder && chunk->blocks[blockIndex - Chunk::Length] == 0))
          {
            // Add left face

            std::array<int, 4> adjacentBlocks = GetAdjacentBlocks(position, BlockSide::Left);

            verticesData.push_back(packVertex(x, y, z, 3, 0, fBlock.textures[3], adjacentBlocks[0]));
            verticesData.push_back(packVertex(x, y, z, 3, 1, fBlock.textures[3], adjacentBlocks[1]));
            verticesData.push_back(packVertex(x, y, z, 3, 2, fBlock.textures[3], adjacentBlocks[2]));
            verticesData.push_back(packVertex(x, y, z, 3, 2, fBlock.textures[3], adjacentBlocks[2]));
            verticesData.push_back(packVertex(x, y, z, 3, 1, fBlock.textures[3], adjacentBlocks[1]));
            verticesData.push_back(packVertex(x, y, z, 3, 3, fBlock.textures[3], adjacentBlocks[3]));
          }

          // Check top face
          bool isTopBorder = z == Chunk::Height - 1;
          if (isTopBorder || chunk->blocks[blockIndex + Chunk::LayerBlocksNumber] == 0)
          {
            // Add top face

            std::array<int, 4> adjacentBlocks = GetAdjacentBlocks(position, BlockSide::Top);

            verticesData.push_back(packVertex(x, y, z, 4, 0, fBlock.textures[4], adjacentBlocks[0]));
            verticesData.push_back(packVertex(x, y, z, 4, 1, fBlock.textures[4], adjacentBlocks[1]));
            verticesData.push_back(packVertex(x, y, z, 4, 2, fBlock.textures[4], adjacentBlocks[2]));
            verticesData.push_back(packVertex(x, y, z, 4, 2, fBlock.textures[4], adjacentBlocks[2]));
            verticesData.push_back(packVertex(x, y, z, 4, 1, fBlock.textures[4], adjacentBlocks[1]));
            verticesData.push_back(packVertex(x, y, z, 4, 3, fBlock.textures[4], adjacentBlocks[3]));
          }

          // Check bottom face
          bool isBottomBorder = z == 0;
          if (isBottomBorder || chunk->blocks[blockIndex - Chunk::LayerBlocksNumber] == 0)
          {
            // Add bottom face

            std::array<int, 4> adjacentBlocks = GetAdjacentBlocks(position, BlockSide::Bottom);

            verticesData.push_back(packVertex(x, y, z, 5, 0, fBlock.textures[5], adjacentBlocks[0]));
            verticesData.push_back(packVertex(x, y, z, 5, 1, fBlock.textures[5], adjacentBlocks[1]));
            verticesData.push_back(packVertex(x, y, z, 5, 2, fBlock.textures[5], adjacentBlocks[2]));
            verticesData.push_back(packVertex(x, y, z, 5, 2, fBlock.textures[5], adjacentBlocks[2]));
            verticesData.push_back(packVertex(x, y, z, 5, 1, fBlock.textures[5], adjacentBlocks[1]));
            verticesData.push_back(packVertex(x, y, z, 5, 3, fBlock.textures[5], adjacentBlocks[3]));
          }
        }
      }
    }

    return verticesData;
  }


  static const glm::ivec3 offsets[6][8] =
  {
    { glm::ivec3(1, -1, 0), glm::ivec3(1, -1, -1), glm::ivec3(1, 0, -1), glm::ivec3(1, 1, -1), glm::ivec3(1, 1, 0), glm::ivec3(1, 1, 1), glm::ivec3(1, 0, 1), glm::ivec3(1, -1, 1) },
    { glm::ivec3(-1, 1, 0), glm::ivec3(-1, 1, -1), glm::ivec3(-1, 0, -1), glm::ivec3(-1, -1, -1), glm::ivec3(-1, -1, 0), glm::ivec3(-1, -1, 1), glm::ivec3(-1, 0, 1), glm::ivec3(-1, 1, 1) },
    { glm::ivec3(1, 1, 0), glm::ivec3(1, 1, -1), glm::ivec3(0, 1, -1), glm::ivec3(-1, 1, -1), glm::ivec3(-1, 1, 0), glm::ivec3(-1, 1, 1), glm::ivec3(0, 1, 1), glm::ivec3(1, 1, 1) },
    { glm::ivec3(-1, -1, 0), glm::ivec3(-1, -1, -1), glm::ivec3(0, -1, -1), glm::ivec3(1, -1, -1), glm::ivec3(1, -1, 0), glm::ivec3(1, -1, 1), glm::ivec3(0, -1, 1), glm::ivec3(-1, -1, 1) },
    { glm::ivec3(0, 1, 1), glm::ivec3(-1, 1, 1), glm::ivec3(-1, 0, 1), glm::ivec3(-1, -1, 1), glm::ivec3(0, -1, 1), glm::ivec3(1, -1, 1), glm::ivec3(1, 0, 1), glm::ivec3(1, 1, 1) },
    { glm::ivec3(0, -1, -1), glm::ivec3(-1, -1, -1), glm::ivec3(-1, 0, -1), glm::ivec3(-1, 1, -1), glm::ivec3(0, 1, -1), glm::ivec3(1, 1, -1), glm::ivec3(1, 0, -1), glm::ivec3(1, -1, -1) }
  };

  std::array<int, 4> OpenglChunkBuilder::GetAdjacentBlocks(glm::ivec3 position, BlockSide side)
  {
    int sideIndex = static_cast<int>(side);

    int blocks[8]{};
    for (int i = 0; i < 8; i++)
    {
      glm::ivec3 offset = offsets[sideIndex][i];
      blocks[i] = GetBlock(position + offset);
    }

    return
    {
      blocks[0] + blocks[2] == 0 ? 0 : blocks[0] + blocks[1] + blocks[2],
      blocks[2] + blocks[4] == 0 ? 0 : blocks[2] + blocks[3] + blocks[4],
      blocks[6] + blocks[0] == 0 ? 0 : blocks[6] + blocks[7] + blocks[0],
      blocks[4] + blocks[6] == 0 ? 0 : blocks[4] + blocks[5] + blocks[6]
    };
  }

  int OpenglChunkBuilder::GetBlock(glm::ivec3 position)
  {
    if (position.z < 0 || position.z >= Chunk::Height)
    {
      return 1;
    }

    glm::ivec2 chunkPosition(1, 1);

    if (position.x < 0)
    {
      position.x += Chunk::Length;
      chunkPosition.x = 0;
    }
    else if (position.x >= Chunk::Length)
    {
      position.x -= Chunk::Length;
      chunkPosition.x = 2;
    }
    if (position.y < 0)
    {
      position.y += Chunk::Width;
      chunkPosition.y = 0;
    }
    else if (position.y >= Chunk::Width)
    {
      position.y -= Chunk::Width;
      chunkPosition.y = 2;
    }

    size_t blockIndex = Chunk::CalculateBlockIndex(static_cast<glm::uvec3>(position));
    if (chunksGrid_[chunkPosition.x][chunkPosition.y]->blocks[blockIndex] == 0)
    {
      return 1;
    }

    return 0;
  }
}
