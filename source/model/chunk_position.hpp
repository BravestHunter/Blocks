#pragma once

#include <utility>


namespace blocks
{
  struct ChunkPosition
  {
    std::int32_t x;
    std::int32_t y;

    bool operator==(const ChunkPosition& other) const
    {
      return x == other.x && y == other.y;
    }
  };

  struct ChunkPositionHasher
  {
    std::size_t operator()(const blocks::ChunkPosition& position) const
    {
      // Source:
      // https://stackoverflow.com/questions/919612/mapping-two-integers-to-one-in-a-unique-and-deterministic-way
      // http://szudzik.com/ElegantPairing.pdf
      std::int32_t a = position.x >= 0 ? 2 * position.x : -2 * position.x - 1;
      std::int32_t b = position.y >= 0 ? 2 * position.y : -2 * position.y - 1;
      std::size_t hash = a >= b ? a * a + a + b : a + b * b;

      return hash;
    }
  };
}
