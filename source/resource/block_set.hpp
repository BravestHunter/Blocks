#pragma once

#include <vector>

#include "block_info.hpp"


class BlockSet
{
public:
  BlockSet(int resolution);

  int GetResolution();

  void AddBlockInfo(BlockInfo blockInfo);
  BlockInfo GetBlockInfo(int index);

  void AddTexture(std::string path);
  std::string GetTexture(int index);
  size_t GetTexturesNumber();

private:
  int resolution_;
  std::vector<BlockInfo> blocks_;
  std::vector<std::string> textures_;
};
