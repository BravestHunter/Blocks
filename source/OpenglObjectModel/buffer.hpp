#pragma once

#include "glew_headers.hpp"
#include "object.hpp"


namespace opengl
{
  class Buffer : public Object
  {
  public:
    Buffer(GLuint bufferType);
    Buffer(const Buffer&) = delete;
    Buffer(Buffer&& other);
    Buffer& operator=(const Buffer&) = delete;
    Buffer& operator=(Buffer&& other);
    virtual ~Buffer() override;

    void SetData(GLsizeiptr size, const void* data);
    void Bind();

  private:
    void Release();

    GLuint bufferType_;
  };
}
