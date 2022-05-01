#pragma once

#include <vector>

#include "glew_headers.hpp"


class OpenglTextureArray
{
public:
  OpenglTextureArray(std::vector<const char*> paths, int resolutionX, int resolutionY);
  ~OpenglTextureArray();

private:
  unsigned int id_;
};
