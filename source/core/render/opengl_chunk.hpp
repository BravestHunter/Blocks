#pragma once

#include <memory>

#include "buffer.hpp"
#include "vertex_array_object.hpp"


namespace blocks
{
  class OpenglChunk
  {
  public:
    OpenglChunk(std::shared_ptr<opengl::Buffer> vbo, std::shared_ptr<opengl::VertexArrayObject> vao, size_t verticesNumber);
    ~OpenglChunk();


    size_t verticesNumber_;
    std::shared_ptr<opengl::VertexArrayObject> vao_;

  private:
    std::shared_ptr<opengl::Buffer> vbo_;
  };
}
