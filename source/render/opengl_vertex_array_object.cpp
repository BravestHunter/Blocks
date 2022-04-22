#include "opengl_vertex_array_object.hpp"


OpenglVertexArrayObject::OpenglVertexArrayObject()
{
  glGenVertexArrays(1, &id_);
}

OpenglVertexArrayObject::~OpenglVertexArrayObject()
{
  glDeleteVertexArrays(1, &id_);
}


void OpenglVertexArrayObject::Bind()
{
  glBindVertexArray(id_);
}
