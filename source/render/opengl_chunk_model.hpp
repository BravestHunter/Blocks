#pragma once

#include <memory>

#include "opengl_buffer.hpp"
#include "opengl_vertex_array_object.hpp"

class OpenglChunkModel
{
public:
  OpenglChunkModel(std::shared_ptr<OpenglBuffer> vbo, std::shared_ptr<OpenglVertexArrayObject> vao, int verticesNumber);
  ~OpenglChunkModel();


  int verticesNumber_;
  std::shared_ptr<OpenglVertexArrayObject> vao_;

private:
  std::shared_ptr<OpenglBuffer> vbo_;
};
