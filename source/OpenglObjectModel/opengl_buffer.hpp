#pragma once

#include "glew_headers.hpp"


namespace opengl
{
  class Buffer
  {
  public:
    Buffer(GLuint bufferType);
    Buffer(const Buffer&) = delete;
    Buffer(Buffer&& other);
    Buffer& operator=(const Buffer&) = delete;
    Buffer& operator=(Buffer&& other);
    ~Buffer();

    void SetData(GLsizeiptr size, const void* data);
    void Bind();

  private:
    void Release();

    GLuint id_;
    GLuint bufferType_;
  };
}
