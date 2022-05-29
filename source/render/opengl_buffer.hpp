#pragma once

#include "glew_headers.hpp"


class OpenglBuffer
{
public:
  OpenglBuffer(GLuint bufferType);
  OpenglBuffer(const OpenglBuffer&) = delete;
  OpenglBuffer(OpenglBuffer&& other);
  OpenglBuffer& operator=(const OpenglBuffer&) = delete;
  OpenglBuffer& operator=(OpenglBuffer&& other);
  ~OpenglBuffer();

  void SetData(GLsizeiptr size, const void* data);
  void Bind();

private:
  void Release();

  GLuint id_;
  GLuint bufferType_;
};
