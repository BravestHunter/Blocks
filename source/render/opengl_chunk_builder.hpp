#pragma once

#include <vector>

#include "opengl_block_info.hpp"


class OpenglChunkBuilder
{
public:
  OpenglChunkBuilder();
  ~OpenglChunkBuilder();

  std::vector<OpenglBlockInfo> blocksIndex_;

private:
};
