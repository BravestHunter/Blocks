#pragma once

#include <string>

#include "glew_headers.hpp"
#include "image.hpp"


namespace opengl
{
  class Texture2D
  {
  public:
    Texture2D(const blocks::Image& image);
    Texture2D(const Texture2D&) = delete;
    Texture2D(Texture2D&& other);
    Texture2D& operator=(const Texture2D&) = delete;
    Texture2D& operator=(Texture2D&& other);
    ~Texture2D();

    void Bind(int slot);

  private:
    void Release();

    GLuint id_;
  };
}
