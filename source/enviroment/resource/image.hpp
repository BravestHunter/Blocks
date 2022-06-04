#pragma once

#include <vector>


namespace blocks
{
  struct Image
  {
    std::vector<unsigned char> data;
    int width;
    int height;
    int channels;
  };
}
