#pragma once

#include <memory>

#include "entt/entt.hpp"

#include "world_data.hpp"
#include "map.hpp"


namespace blocks
{
  class World
  {
  public:
    World(WorldData worldData);
    ~World();

    inline std::string GetName() const;
    inline std::shared_ptr<Map> GetMap();
    inline entt::registry& GetEcsRegistry();
    inline entt::entity GetPlayerEntity() const;

  private:
    std::string path_;
    std::string name_;
    std::shared_ptr<Map> map_;
    entt::registry ecsRegistry_;
    entt::entity playerEntity_;

    static void Save(WorldData worldData, std::string path);
  };


  std::string World::GetName() const
  {
    return name_;
  }

  std::shared_ptr<Map> World::GetMap()
  {
    return map_;
  }

  entt::registry& World::GetEcsRegistry()
  {
    return ecsRegistry_;
  }

  entt::entity World::GetPlayerEntity() const
  {
    return playerEntity_;
  }
}
