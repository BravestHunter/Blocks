#pragma once

#include <string>
#include <vector>

#include "glew_headers.hpp"
#include "image.hpp"


namespace opengl
{
  class Texture2DArray
  {
  public:
    Texture2DArray(const std::vector<blocks::Image>& images, const int resolutionX, const int resolutionY);
    Texture2DArray(const Texture2DArray&) = delete;
    Texture2DArray(Texture2DArray&& other);
    Texture2DArray& operator=(const Texture2DArray&) = delete;
    Texture2DArray& operator=(Texture2DArray&& other);
    ~Texture2DArray();

    void Bind(int slot);

  private:
    void Release();

    GLuint id_;
  };
}
