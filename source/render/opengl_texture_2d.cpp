#include "opengl_texture_2d.hpp"

#include <iostream>

#include "io/file_api.hpp"


OpenglTexture2D::OpenglTexture2D(const Image& image)
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

OpenglTexture2D::OpenglTexture2D(OpenglTexture2D&& other) : id_(other.id_)
{
  other.id_ = 0;
}

OpenglTexture2D& OpenglTexture2D::operator=(OpenglTexture2D&& other)
{
  if (this != &other)
  {
    Release();
    std::swap(id_, other.id_);
  }

  return *this;
}

OpenglTexture2D::~OpenglTexture2D()
{
  Release();
}


void OpenglTexture2D::Bind(int slot)
{
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, id_);
}


void OpenglTexture2D::Release()
{
  glDeleteTextures(1, &id_);
  id_ = 0;
}
