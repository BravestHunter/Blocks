#pragma once

#include "glew_headers.hpp"
#include "object.hpp"


namespace opengl
{
  class VertexArrayObject : public Object
  {
  public:
    VertexArrayObject();
    VertexArrayObject(const VertexArrayObject&) = delete;
    VertexArrayObject(VertexArrayObject&& other);
    VertexArrayObject& operator=(const VertexArrayObject&) = delete;
    VertexArrayObject& operator=(VertexArrayObject&& other);
    virtual ~VertexArrayObject() override;

    void Bind();

  private:
    void Release();
  };
}
