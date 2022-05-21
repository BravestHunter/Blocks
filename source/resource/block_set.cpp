#include "block_set.hpp"


BlockSet::BlockSet(int resolution) : resolution_(resolution), blocks_()
{
}


int BlockSet::GetResolution()
{
  return resolution_;
}


void BlockSet::AddBlockInfo(BlockInfo blockInfo)
{
  blocks_.push_back(blockInfo);
}

BlockInfo BlockSet::GetBlockInfo(int index)
{
  return blocks_.at(index);
}


void BlockSet::AddTexture(std::string path)
{
  textures_.push_back(path);
}

std::string BlockSet::GetTexture(int index)
{
  return textures_.at(index);
}

size_t BlockSet::GetTexturesNumber()
{
  return textures_.size();
}
