#pragma once

#include "glew_headers.hpp"


class OpenglBuffer
{
public:
  OpenglBuffer(unsigned int bufferType);
  ~OpenglBuffer();

  void SetData(GLsizeiptr size, const void* data);
  void Bind();

private:
  unsigned int id_;
  unsigned int bufferType_;
};
