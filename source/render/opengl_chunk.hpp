#pragma once

#include <memory>

#include "render/opengl_buffer.hpp"
#include "render/opengl_vertex_array_object.hpp"


namespace blocks
{
  class OpenglChunk
  {
  public:
    OpenglChunk(std::shared_ptr<OpenglBuffer> vbo, std::shared_ptr<OpenglVertexArrayObject> vao, int verticesNumber);
    ~OpenglChunk();


    int verticesNumber_;
    std::shared_ptr<OpenglVertexArrayObject> vao_;

  private:
    std::shared_ptr<OpenglBuffer> vbo_;
  };
}
