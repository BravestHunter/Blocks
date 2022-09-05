#include "vertex_array_object.hpp"

#include <utility>


namespace opengl
{
  VertexArrayObject::VertexArrayObject()
  {
    glGenVertexArrays(1, &id_);
  }

  VertexArrayObject::VertexArrayObject(VertexArrayObject&& other) : Object(other.id_)
  {
    other.id_ = 0;
  }

  VertexArrayObject& VertexArrayObject::operator=(VertexArrayObject&& other)
  {
    if (this == &other)
    {
      return *this;
    }

    Release();
    std::swap(id_, other.id_);

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
    if (id_ != 0)
    {
      glDeleteVertexArrays(1, &id_);
      id_ = 0;
    }
  }
}
