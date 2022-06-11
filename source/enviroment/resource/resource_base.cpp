#include "resource_base.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "nlohmann/json.hpp"

#include "io/file_api.hpp"


namespace blocks
{
  ResourceBase::ResourceBase()
  {
    blockSetPaths_ = std::make_shared<std::vector<std::string>>();
  }

  ResourceBase::~ResourceBase()
  {

  }


  void ResourceBase::SetUp(std::string path)
  {
    if (!blocks::isPathExist(path))
    {
      return;
    }

    size_t found = path.find_last_of("/\\");
    rootDirectory_ = (path.substr(0, found));

    std::string resourceBaseStr = blocks::readTextFile(path);

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

    std::string blockSetStr = blocks::readTextFile(bsFile);
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


  Image ResourceBase::ReadImage(std::string path)
  {
    Image result;

    std::vector<unsigned char> data = blocks::readBinaryFile(path);

    stbi_set_flip_vertically_on_load(true);

    unsigned char* rawData = stbi_load_from_memory(&data[0], data.size(), &result.width, &result.height, &result.channels, 0);
    result.data = std::vector<unsigned char>(rawData, rawData + result.height * result.width * result.channels);
    stbi_image_free(rawData);

    return result;
  }
}
