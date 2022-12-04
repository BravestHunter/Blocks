#pragma once

#include <memory>

#include "resource/model.hpp"
#include "buffer.hpp"
#include "vertex_array_object.hpp"
#include "opengl_sprite.hpp"
#include "texture_2d.hpp"


namespace blocks
{
  class OpenglModel
  {
  public:
    OpenglModel(std::shared_ptr<opengl::VertexArrayObject> vao, std::shared_ptr<opengl::Buffer> vbo, std::shared_ptr<opengl::Buffer> ebo, size_t indicesCount);
    OpenglModel(const Model& model);
    OpenglModel(const OpenglModel&) = delete;
    OpenglModel(OpenglModel&& other) = delete;
    OpenglModel& operator=(const OpenglModel&) = delete;
    OpenglModel& operator=(OpenglModel&& other) = delete;
    ~OpenglModel() {}

    const std::shared_ptr<opengl::VertexArrayObject> GetVao() const;
    size_t GetIndicesCount() const;
    const std::shared_ptr<opengl::Texture2D> GetTexture() const;

  private:
    std::shared_ptr<opengl::VertexArrayObject> vao_;
    std::shared_ptr<opengl::Buffer> vbo_;
    std::shared_ptr<opengl::Buffer> ebo_;
    size_t indicesCount_;

    std::shared_ptr<opengl::Texture2D> texture_;
  };


  inline const std::shared_ptr<opengl::VertexArrayObject> OpenglModel::GetVao() const
  {
    return vao_;
  }

  inline size_t OpenglModel::GetIndicesCount() const
  {
    return indicesCount_;
  }

  inline const std::shared_ptr<opengl::Texture2D> OpenglModel::GetTexture() const
  {
    return texture_;
  }
}
