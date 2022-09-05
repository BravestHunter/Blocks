#include "opengl_texture_2d.hpp"

#include <iostream>

#include "io/file_api.hpp"


namespace opengl
{
  Texture2D::Texture2D(const blocks::Image& image)
  {
    glGenTextures(1, &id_);
    glBindTexture(GL_TEXTURE_2D, id_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if (!image.data.empty())
    {
      GLuint channelsMode = GL_RGB;
      if (image.channels == 4)
        channelsMode = GL_RGBA;

      glTexImage2D(GL_TEXTURE_2D, 0, channelsMode, image.width, image.height, 0, channelsMode, GL_UNSIGNED_BYTE, &image.data[0]);
      glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
      std::cout << "Failed to load texture" << std::endl;
    }
  }

  Texture2D::Texture2D(Texture2D&& other) : id_(other.id_)
  {
    other.id_ = 0;
  }

  Texture2D& Texture2D::operator=(Texture2D&& other)
  {
    if (this != &other)
    {
      Release();
      std::swap(id_, other.id_);
    }

    return *this;
  }

  Texture2D::~Texture2D()
  {
    Release();
  }


  void Texture2D::Bind(int slot)
  {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, id_);
  }


  void Texture2D::Release()
  {
    glDeleteTextures(1, &id_);
    id_ = 0;
  }
}
