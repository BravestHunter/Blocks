#pragma once

#include <string>
#include <memory>
#include <vector>

#include "block_set.hpp"

class ResourceBase
{
public:
  ResourceBase();

  bool SetUp(std::string path);
  std::shared_ptr<std::vector<std::string>> GetBlockSetNames();
  std::shared_ptr<BlockSet> LoadBlockSet(std::string name);

private:
  std::string rootDirectory_;
  std::shared_ptr<std::vector<std::string>> blockSetPaths_;
};
