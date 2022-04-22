#pragma once

#include "glew_headers.hpp"


class OpenglTexture
{
public:
  OpenglTexture(const char* path);
  ~OpenglTexture();

  void Bind(int slot);

private:
  unsigned int id_;
};
