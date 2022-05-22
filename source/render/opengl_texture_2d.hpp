#pragma once

#include <string>

#include "glew_headers.hpp"


class OpenglTexture2D
{
public:
  OpenglTexture2D(const std::string path);
  OpenglTexture2D(const OpenglTexture2D&) = delete;
  OpenglTexture2D(OpenglTexture2D&& other);
  OpenglTexture2D& operator=(const OpenglTexture2D&) = delete;
  OpenglTexture2D& operator=(OpenglTexture2D&& other);
  ~OpenglTexture2D();

  void Bind(int slot);

private:
  void Release();

  unsigned int id_;
};
