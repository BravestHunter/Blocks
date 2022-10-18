#pragma once

#include <memory>

#include "buffer.hpp"
#include "vertex_array_object.hpp"


namespace blocks
{
  class OpenglModel
  {
  public:
    OpenglModel(std::shared_ptr<opengl::VertexArrayObject> vao, std::shared_ptr<opengl::Buffer> vbo, std::shared_ptr<opengl::Buffer> ebo, size_t verticesNumber);
    OpenglModel(const OpenglModel&) = delete;
    OpenglModel(OpenglModel&& other) = delete;
    OpenglModel& operator=(const OpenglModel&) = delete;
    OpenglModel& operator=(OpenglModel&& other) = delete;
    ~OpenglModel() {}

    inline const std::shared_ptr<opengl::VertexArrayObject> GetVao() const;
    inline size_t GetNumber() const;

  private:
    std::shared_ptr<opengl::VertexArrayObject> vao_;
    std::shared_ptr<opengl::Buffer> vbo_;
    std::shared_ptr<opengl::Buffer> ebo_;
    size_t verticesNumber_;
  };


  const std::shared_ptr<opengl::VertexArrayObject> OpenglModel::GetVao() const
  {
    return vao_;
  }

  size_t OpenglModel::GetNumber() const
  {
    return verticesNumber_;
  }
}
