#include "opengl_buffer.hpp"

#include <utility>


namespace blocks
{
  OpenglBuffer::OpenglBuffer(GLuint bufferType) : bufferType_(bufferType)
  {
    glGenBuffers(1, &id_);
  }

  OpenglBuffer::OpenglBuffer(OpenglBuffer&& other) : id_(other.id_)
  {
    other.id_ = 0;
  }

  OpenglBuffer& OpenglBuffer::operator=(OpenglBuffer&& other)
  {
    if (this != &other)
    {
      Release();
      std::swap(id_, other.id_);
    }

    return *this;
  }

  OpenglBuffer::~OpenglBuffer()
  {
    Release();
  }


  void OpenglBuffer::SetData(GLsizeiptr size, const void* data)
  {
    glBufferData(bufferType_, size, data, GL_STATIC_DRAW);
  }

  void OpenglBuffer::Bind()
  {
    glBindBuffer(bufferType_, id_);
  }


  void OpenglBuffer::Release()
  {
    glDeleteBuffers(1, &id_);
    id_ = 0;
  }
}
