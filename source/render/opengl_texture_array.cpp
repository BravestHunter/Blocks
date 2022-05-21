#include "opengl_texture_array.hpp"

#include <iostream>

#include "io/file_api.hpp"


OpenglTextureArray::OpenglTextureArray(std::vector<std::string> paths, int resolutionX, int resolutionY)
{
  glGenTextures(1, &id_);
  glBindTexture(GL_TEXTURE_2D_ARRAY, id_);
  glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, resolutionX, resolutionY, 6, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

  for (int i = 0; i < paths.size(); i++)
  {
    Image image = readImage(paths[i]);
    if (!image.data.empty())
    {
      GLuint channelsMode = GL_RGB;
      if (image.channels == 4)
        channelsMode = GL_RGBA;

      glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, resolutionX, resolutionY, 1, channelsMode, GL_UNSIGNED_BYTE, &image.data[0]);
      glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
      std::cout << "Failed to load texture" << std::endl;
    }
  }

  glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

OpenglTextureArray::~OpenglTextureArray()
{

}
