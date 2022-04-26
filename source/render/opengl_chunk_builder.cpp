#include "opengl_chunk_builder.hpp"


OpenglChunkBuilder::OpenglChunkBuilder()
{
  blocksIndex_ = std::vector<OpenglBlockInfo>(4);

  blocksIndex_[1] = OpenglBlockInfo({ glm::vec2(0.0f, 0.0f), glm::vec2(0.5f, 0.0f), glm::vec2(0.0f, 0.5f), glm::vec2(0.5f, 0.5f) });
  blocksIndex_[2] = OpenglBlockInfo({ glm::vec2(0.5f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(0.5f, 0.5f), glm::vec2(1.0f, 0.5f) });
  blocksIndex_[3] = OpenglBlockInfo({ glm::vec2(0.0f, 0.5f), glm::vec2(0.5f, 0.5f), glm::vec2(0.0f, 1.0f), glm::vec2(0.5f, 1.0f) });
}

OpenglChunkBuilder::~OpenglChunkBuilder()
{

}
