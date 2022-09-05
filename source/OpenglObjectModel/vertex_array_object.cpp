#include "vertex_array_object.hpp"

#include <utility>


namespace opengl
{
  VertexArrayObject::VertexArrayObject()
  {
    glGenVertexArrays(1, &id_);
  }

  VertexArrayObject::VertexArrayObject(VertexArrayObject&& other) : id_(other.id_)
  {
    other.id_ = 0;
  }

  VertexArrayObject& VertexArrayObject::operator=(VertexArrayObject&& other)
  {
    if (this != &other)
    {
      Release();
      std::swap(id_, other.id_);
    }

    return *this;
  }

  VertexArrayObject::~VertexArrayObject()
  {
    Release();
  }


  void VertexArrayObject::Bind()
  {
    glBindVertexArray(id_);
  }


  void VertexArrayObject::Release()
  {
    glDeleteVertexArrays(1, &id_);
    id_ = 0;
  }
}
