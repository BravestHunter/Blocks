#include "opengl_texture.hpp"

#include <iostream>

#include "io/file_api.hpp"


OpenglTexture::OpenglTexture(const char* path)
{
  glGenTextures(1, &id_);
  glBindTexture(GL_TEXTURE_2D, id_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  Image image = readImage(path);
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

OpenglTexture::~OpenglTexture()
{

}


void OpenglTexture::Bind(int slot)
{
  glActiveTexture(slot);
  glBindTexture(GL_TEXTURE_2D, id_);
}
