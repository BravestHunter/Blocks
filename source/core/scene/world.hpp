#pragma once

#include <memory>

#include "world_data.hpp"
#include "entity.hpp"
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
    inline Entity& GetPlayer();

  private:
    std::string path_;
    std::string name_;
    std::shared_ptr<Map> map_;
    Entity player_;

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

  Entity& World::GetPlayer()
  {
    return player_;
  }
}
