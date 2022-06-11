#pragma once

#include "glew_headers.hpp"


namespace blocks
{
  class OpenglVertexArrayObject
  {
  public:
    OpenglVertexArrayObject();
    OpenglVertexArrayObject(const OpenglVertexArrayObject&) = delete;
    OpenglVertexArrayObject(OpenglVertexArrayObject&& other);
    OpenglVertexArrayObject& operator=(const OpenglVertexArrayObject&) = delete;
    OpenglVertexArrayObject& operator=(OpenglVertexArrayObject&& other);
    ~OpenglVertexArrayObject();

    void Bind();

  private:
    void Release();

    GLuint id_;
  };
}
