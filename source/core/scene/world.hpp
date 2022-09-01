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
    World(WorldData data);
    ~World();

    inline std::string GetName() const;
    inline Entity& GetPlayer();
    std::shared_ptr<Map> GetMap();

  private:
    WorldData data_;
    std::string path_;
    Entity player_;
    std::shared_ptr<Map> map_;
  };


  std::string World::GetName() const
  {
    return std::string(data_.name);
  }

  Entity& World::GetPlayer()
  {
    return player_;
  }
}
