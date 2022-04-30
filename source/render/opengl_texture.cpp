#include "opengl_texture.hpp"

#include <iostream>


OpenglTexture::OpenglTexture(const char* path)
{
  glGenTextures(1, &id_);
  glBindTexture(GL_TEXTURE_2D, id_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  int width, height, nrChannels;
  stbi_set_verticall_flip(true);
  unsigned char* data = load_image(path, &width, &height, &nrChannels, 0);

  if (data)
  {
    GLuint channelsMode = GL_RGB;
    if (nrChannels == 4)
      channelsMode = GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, channelsMode, width, height, 0, channelsMode, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    std::cout << "Failed to load texture" << std::endl;
  }
  free_image(data);
}

OpenglTexture::~OpenglTexture()
{

}


void OpenglTexture::Bind(int slot)
{
  glActiveTexture(slot);
  glBindTexture(GL_TEXTURE_2D, id_);
}
