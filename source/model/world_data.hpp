#pragma once

#include "map_data.hpp"
#include "player_data.hpp"


namespace blocks
{
  struct WorldData
  {
    char name[16];
    MapData mapData;
    PlayerData playerData;
  };
}
