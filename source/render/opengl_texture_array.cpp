#include "opengl_texture_array.hpp"

#include <iostream>


OpenglTextureArray::OpenglTextureArray(std::vector<const char*> paths, int resolutionX, int resolutionY)
{
  glGenTextures(1, &id_);
  glBindTexture(GL_TEXTURE_2D_ARRAY, id_);
  glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, resolutionX, resolutionY, 6, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

  for (int i = 0; i < paths.size(); i++)
  {
    int width, height, nrChannels;
    stbi_set_verticall_flip(true);
    unsigned char* data = load_image(paths[i], &width, &height, &nrChannels, 0);

    if (data)
    {
      GLuint channelsMode = GL_RGB;
      if (nrChannels == 4)
        channelsMode = GL_RGBA;

      glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, resolutionX, resolutionY, 1, channelsMode, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
      std::cout << "Failed to load texture" << std::endl;
    }
    free_image(data);
  }

  glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

OpenglTextureArray::~OpenglTextureArray()
{

}
