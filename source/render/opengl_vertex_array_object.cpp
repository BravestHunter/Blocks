#include "opengl_vertex_array_object.hpp"

#include <utility>


OpenglVertexArrayObject::OpenglVertexArrayObject()
{
  glGenVertexArrays(1, &id_);
}

OpenglVertexArrayObject::OpenglVertexArrayObject(OpenglVertexArrayObject&& other) : id_(other.id_)
{
  other.id_ = 0;
}

OpenglVertexArrayObject& OpenglVertexArrayObject::operator=(OpenglVertexArrayObject&& other)
{
  if (this != &other)
  {
    Release();
    std::swap(id_, other.id_);
  }

  return *this;
}

OpenglVertexArrayObject::~OpenglVertexArrayObject()
{
  Release();
}


void OpenglVertexArrayObject::Bind()
{
  glBindVertexArray(id_);
}


void OpenglVertexArrayObject::Release()
{
  glDeleteVertexArrays(1, &id_);
  id_ = 0;
}
