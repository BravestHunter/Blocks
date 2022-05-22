#pragma once

#include <string>
#include <vector>

#include "glew_headers.hpp"


class OpenglTexture2DArray
{
public:
  OpenglTexture2DArray(const std::vector<std::string> paths, const int resolutionX, const int resolutionY);
  OpenglTexture2DArray(const OpenglTexture2DArray&) = delete;
  OpenglTexture2DArray(OpenglTexture2DArray&& other);
  OpenglTexture2DArray& operator=(const OpenglTexture2DArray&) = delete;
  OpenglTexture2DArray& operator=(OpenglTexture2DArray&& other);
  ~OpenglTexture2DArray();

  void Bind(int slot);

private:
  void Release();

  GLuint id_;
};
