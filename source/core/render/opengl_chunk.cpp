#include "opengl_chunk.hpp"


namespace blocks
{
  OpenglChunk::OpenglChunk(std::shared_ptr<opengl::Buffer> vbo, std::shared_ptr<opengl::VertexArrayObject> vao, size_t verticesNumber)
    : vbo_(vbo), vao_(vao), verticesNumber_(verticesNumber)
  {

  }

  OpenglChunk::~OpenglChunk()
  {

  }
}
