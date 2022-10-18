#include "opengl_model.hpp"


namespace blocks
{
  OpenglModel::OpenglModel(std::shared_ptr<opengl::VertexArrayObject> vao, std::shared_ptr<opengl::Buffer> vbo, std::shared_ptr<opengl::Buffer> ebo, size_t verticesNumber)
    : vao_(vao), vbo_(vbo), ebo_(ebo), verticesNumber_(verticesNumber)
  {
  }
}
