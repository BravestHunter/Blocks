#pragma once

#include <memory>

#include "opengl_buffer.hpp"
#include "opengl_vertex_array_object.hpp"
#include "opengl_texture_2d.hpp"


class OpenglModel
{
public:
  OpenglModel(std::shared_ptr<OpenglBuffer> vbo, std::shared_ptr<OpenglVertexArrayObject> vao, std::shared_ptr<OpenglTexture2D> texture);
  OpenglModel(std::shared_ptr<OpenglBuffer> vbo, std::shared_ptr<OpenglVertexArrayObject> vao, std::shared_ptr<OpenglBuffer> ebo_, std::shared_ptr<OpenglTexture2D> texture);
  ~OpenglModel();

  std::shared_ptr<OpenglBuffer> GetVBO();
  std::shared_ptr<OpenglVertexArrayObject> GetVAO();
  std::shared_ptr<OpenglTexture2D> GetTexture();

private:
  std::shared_ptr<OpenglBuffer> vbo_;
  std::shared_ptr<OpenglVertexArrayObject> vao_;
  std::shared_ptr<OpenglBuffer> ebo_;
  std::shared_ptr<OpenglTexture2D> texture_;
};
