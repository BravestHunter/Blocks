#include "world.hpp"

#include <format>

#include "io/file_api.hpp"
#include "serialization.hpp"


namespace blocks
{
  World::World(WorldData data) :
    data_(data),
    path_(std::format("worlds/{0}", data.name)),
    map_(std::make_shared<Map>(data.mapData, path_))
  {
  }

  World::~World()
  {
    if (!isPathExist("worlds"))
    {
      createDirectory("worlds");
    }

    if (!isPathExist(path_))
    {
      createDirectory(path_);
    }

    std::string worldDataFilePath = std::format("{0}/world.dat", path_);
    std::vector<unsigned char> bytes = serializeWorld(&data_);
    blocks::saveBinaryFile(worldDataFilePath, bytes);
  }


  std::shared_ptr<Map> World::GetMap()
  {
    return map_;
  }
}
