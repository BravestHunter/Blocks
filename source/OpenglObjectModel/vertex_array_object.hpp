#pragma once

#include "glew_headers.hpp"


namespace opengl
{
  class VertexArrayObject
  {
  public:
    VertexArrayObject();
    VertexArrayObject(const VertexArrayObject&) = delete;
    VertexArrayObject(VertexArrayObject&& other);
    VertexArrayObject& operator=(const VertexArrayObject&) = delete;
    VertexArrayObject& operator=(VertexArrayObject&& other);
    ~VertexArrayObject();

    void Bind();

  private:
    void Release();

    GLuint id_;
  };
}
