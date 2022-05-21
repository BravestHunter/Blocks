#pragma once

#include <string>
#include <vector>

#include "glew_headers.hpp"


class OpenglTextureArray
{
public:
  OpenglTextureArray(std::vector<std::string> paths, int resolutionX, int resolutionY);
  ~OpenglTextureArray();

private:
  unsigned int id_;
};
