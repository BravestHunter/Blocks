#pragma once

#include <vector>


struct Image
{
  std::vector<unsigned char> data;
  int width;
  int height;
  int channels;
};
