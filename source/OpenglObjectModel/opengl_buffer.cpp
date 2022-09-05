#include "opengl_buffer.hpp"

#include <utility>


namespace opengl
{
  Buffer::Buffer(GLuint bufferType) : bufferType_(bufferType)
  {
    glGenBuffers(1, &id_);
  }

  Buffer::Buffer(Buffer&& other) : id_(other.id_)
  {
    other.id_ = 0;
  }

  Buffer& Buffer::operator=(Buffer&& other)
  {
    if (this != &other)
    {
      Release();
      std::swap(id_, other.id_);
    }

    return *this;
  }

  Buffer::~Buffer()
  {
    Release();
  }


  void Buffer::SetData(GLsizeiptr size, const void* data)
  {
    glBufferData(bufferType_, size, data, GL_STATIC_DRAW);
  }

  void Buffer::Bind()
  {
    glBindBuffer(bufferType_, id_);
  }


  void Buffer::Release()
  {
    glDeleteBuffers(1, &id_);
    id_ = 0;
  }
}
