#pragma once

#include <memory>

#include "world_data.hpp"
#include "map.hpp"


namespace blocks
{
  class World
  {
  public:
    World(WorldData data);
    ~World();

    std::shared_ptr<Map> GetMap();

  private:
    WorldData data_;
    std::string path_;
    std::shared_ptr<Map> map_;
  };
}
