#pragma once

#include "glew_headers.hpp"
#include "io/stb_loader.hpp"


class OpenglTexture
{
public:
  OpenglTexture(const char* path);
  ~OpenglTexture();

  void Bind(int slot);

private:
  unsigned int id_;
};
