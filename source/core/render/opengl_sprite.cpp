#include "opengl_sprite.hpp"


namespace blocks
{
  OpenglSprite::OpenglSprite(Image image)
  {
    vbo_ = std::make_shared<opengl::Buffer>(GL_ARRAY_BUFFER);
    vao_ = std::make_shared<opengl::VertexArrayObject>();

    float positions[24] =
    {
      0.0f, 0.0f, 0.0f, 1.0f,
      0.0f, 1.0f, 0.0f, 0.0f,
      1.0f, 1.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f,
      1.0f, 1.0f, 1.0f, 0.0f,
      1.0f, 0.0f, 1.0f, 1.0f
    };

    vao_->Bind();
    vbo_->Bind();
    vbo_->SetData(sizeof(float) * 24, &positions);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    texture_ = std::make_shared<opengl::Texture2D>(image);
  }
}
