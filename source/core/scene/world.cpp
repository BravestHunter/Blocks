#include "world.hpp"

#include <format>

#include "io/file_api.hpp"
#include "serialization.hpp"
#include "ecs/components/transform.hpp"
#include "ecs/components/physics_body.hpp"


namespace blocks
{
  World::World(WorldData worldData) :
    path_(std::format("worlds/{0}", worldData.name)),
    name_(worldData.name),
    map_(std::make_shared<Map>(worldData.mapData, path_))
  {
    playerEntity_ = ecsRegistry_.create();

    Transform transform
    {
      .position = worldData.playerData.position
    };
    ecsRegistry_.emplace<Transform>(playerEntity_, transform);

    PhysicsBody physicsBody
    {
      .bounds = AABB(glm::vec3(0.0f), glm::vec3(0.5f, 0.5f, 1.0f))
    };
    ecsRegistry_.emplace<PhysicsBody>(playerEntity_, physicsBody);
  }

  World::~World()
  {
    Transform playerTransform = ecsRegistry_.get<Transform>(playerEntity_);

    WorldData worldData
    {
      .mapData = MapData
      {
        .seed = map_->GetSeed()
      },
      .playerData = PlayerData
      {
        .position = playerTransform.position
      }
    };
    
    strcpy(worldData.name, name_.c_str());

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
