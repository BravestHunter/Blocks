#pragma once

#include <utility>

#include <glm/glm.hpp>


enum class Direction
{
  Forward,
  Back,
  Right,
  Left,
  Up,
  Down
};

struct BlockLookAt
{
  bool hit = false;
  std::pair<int, int> chunkPosition;
  glm::ivec3 blockPosition;
  Direction loockFromDirection;
};
