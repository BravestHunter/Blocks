#pragma once

#include "map_data.hpp"
#include "player_data.hpp"


namespace blocks
{
  struct WorldData
  {
    MapData mapData;
    PlayerData playerData;
  };
}
