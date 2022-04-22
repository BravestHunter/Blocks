#include "opengl_buffer.hpp"


OpenglBuffer::OpenglBuffer(unsigned int bufferType) : bufferType_(bufferType)
{
  glGenBuffers(1, &id_);
}

OpenglBuffer::~OpenglBuffer()
{
  glDeleteBuffers(1, &id_);
}


void OpenglBuffer::SetData(GLsizeiptr size, const void* data)
{
  glBufferData(bufferType_, size, data, GL_STATIC_DRAW);
}

void OpenglBuffer::Bind()
{
  glBindBuffer(bufferType_, id_);
}
