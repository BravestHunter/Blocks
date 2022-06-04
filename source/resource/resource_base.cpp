#include "resource_base.hpp"

#include "nlohmann/json.hpp"

#include "io/file_api.hpp"


ResourceBase::ResourceBase()
{
  blockSetPaths_ = std::make_shared<std::vector<std::string>>();
}

ResourceBase::~ResourceBase()
{

}


void ResourceBase::Init()
{
  // Nothing to do here
}

void ResourceBase::Deinit()
{
  // Nothing to do here
}

bool ResourceBase::IsInitialized()
{
  return true;
}



void ResourceBase::SetUp(std::string path)
{
  if (!isPathExist(path))
  {
    return;
  }

  size_t found = path.find_last_of("/\\");
  rootDirectory_ = (path.substr(0, found));

  std::string resourceBaseStr = readTextFile(path);

  nlohmann::json resourceBaseJson = nlohmann::json::parse(resourceBaseStr);
  nlohmann::json blockSets = resourceBaseJson["BlockSets"];
  for (auto& [key, value] : blockSets.items())
  {
    blockSetPaths_->push_back(value["Name"]);
  }
}


std::shared_ptr<std::vector<std::string>> ResourceBase::GetBlockSetNames()
{
  return blockSetPaths_;
}

std::shared_ptr<BlockSet> ResourceBase::LoadBlockSet(std::string name)
{
  std::string bsDirectory = rootDirectory_ + "\\" + "BlockSets" + "\\" + name;
  std::string bsFile = bsDirectory + "\\" + name + ".bs";

  std::string blockSetStr = readTextFile(bsFile);
  nlohmann::json resourceBaseJson = nlohmann::json::parse(blockSetStr);
  nlohmann::json blocksJson = resourceBaseJson["Blocks"];

  std::shared_ptr<BlockSet> blockSet = std::make_shared<BlockSet>(resourceBaseJson["Resolution"]);

  for (auto& [key, value] : blocksJson.items())
  {
    BlockInfo blockInfo;
    blockInfo.name = value["Name"];

    nlohmann::json blockTextures = value["Textures"];
    for (auto& [key, value] : blockTextures.items())
    {
      int index = std::stoi(key);
      blockInfo.textures[index] = value;
    }

    blockSet->AddBlockInfo(blockInfo);
  }

  int texturesNumber = resourceBaseJson["TexturesNumber"];
  for (int i = 0; i < texturesNumber; i++)
  {
    blockSet->AddTexture(bsDirectory + "\\" + std::to_string(i) + ".png");
  }

  return blockSet;
}