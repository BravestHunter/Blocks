#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "image.hpp"
#include "buffer.hpp"
#include "vertex_array_object.hpp"
#include "texture_2d.hpp"


namespace blocks
{
  class OpenglSprite
  {
  public:
    OpenglSprite(Image image);

    inline std::shared_ptr<opengl::VertexArrayObject> GetVao();
    inline std::shared_ptr<opengl::Texture2D> GetTexture();

  private:
    std::shared_ptr<opengl::Buffer> vbo_;
    std::shared_ptr<opengl::VertexArrayObject> vao_;
    std::shared_ptr<opengl::Texture2D> texture_;
  };


  std::shared_ptr<opengl::VertexArrayObject> OpenglSprite::GetVao()
  {
    return vao_;
  }

  std::shared_ptr<opengl::Texture2D> OpenglSprite::GetTexture()
  {
    return texture_;
  }
}
