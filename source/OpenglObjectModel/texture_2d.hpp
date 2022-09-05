#pragma once

#include <string>

#include "image.hpp"
#include "glew_headers.hpp"
#include "object.hpp"


namespace opengl
{
  class Texture2D : public Object
  {
  public:
    Texture2D(const blocks::Image& image);
    Texture2D(const Texture2D&) = delete;
    Texture2D(Texture2D&& other);
    Texture2D& operator=(const Texture2D&) = delete;
    Texture2D& operator=(Texture2D&& other);
    virtual ~Texture2D() override;

    void Bind(int slot);

  private:
    void Release();
  };
}
