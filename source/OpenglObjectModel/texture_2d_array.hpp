#pragma once

#include <string>
#include <vector>

#include "image.hpp"
#include "glew_headers.hpp"
#include "object.hpp"


namespace opengl
{
  class Texture2DArray : public Object
  {
  public:
    Texture2DArray(const std::vector<blocks::Image>& images, const int resolutionX, const int resolutionY);
    Texture2DArray(const Texture2DArray&) = delete;
    Texture2DArray(Texture2DArray&& other);
    Texture2DArray& operator=(const Texture2DArray&) = delete;
    Texture2DArray& operator=(Texture2DArray&& other);
    virtual ~Texture2DArray() override;

    void Bind(int slot);

  private:
    void Release();
  };
}
