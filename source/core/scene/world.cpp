#include "world.hpp"

#include <format>

#include "io/file_api.hpp"
#include "serialization.hpp"


namespace blocks
{
  World::World(WorldData worldData) :
    path_(std::format("worlds/{0}", worldData.name)),
    name_(worldData.name),
    player_(worldData.playerData.position, AABB(glm::vec3(0.0f), glm::vec3(0.5f, 0.5f, 2.0f))),
    map_(std::make_shared<Map>(worldData.mapData, path_))
  {
  }

  World::~World()
  {
    WorldData worldData
    {
      .mapData = MapData
      {
        .seed = map_->GetSeed()
      },
      .playerData = PlayerData
      {
        .position = player_.GetPosition()
      }
    };
    
    Save(worldData, path_);
  }


  void World::Save(WorldData worldData, std::string path)
  {
    if (!isPathExist("worlds"))
    {
      createDirectory("worlds");
    }

    if (!isPathExist(path))
    {
      createDirectory(path);
    }

    std::string worldDataFilePath = std::format("{0}/world.dat", path);
    std::vector<unsigned char> bytes = serializeWorld(worldData);
    blocks::saveBinaryFile(worldDataFilePath, bytes);
  }
}
