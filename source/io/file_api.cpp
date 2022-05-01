#include "file_api.hpp"

#include <fstream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"


std::vector<unsigned char> readBinaryFile(std::string path)
{
  std::ifstream inputStream;
  std::vector<unsigned char> result;

  //input.exceptions(std::ifstream::eofbit | std::ifstream::failbit | std::ifstream::badbit);

  inputStream.open(path, std::ios::in | std::ios::binary);
  if (inputStream)
  {
    result = std::vector<unsigned char>(std::istreambuf_iterator<char>(inputStream), {});
  }
  inputStream.close();

  return result;
}

std::string readTextFile(std::string path)
{
  std::ifstream inputStream;
  std::string result;

  inputStream.open(path, std::ios::in);
  if (inputStream)
  {
    std::stringstream stringStream;
    stringStream << inputStream.rdbuf();

    result = stringStream.str();
  }
  inputStream.close();

  return result;
}

Image readImage(std::string path)
{
  Image result;

  std::vector<unsigned char> data = readBinaryFile(path);

  stbi_set_flip_vertically_on_load(true);

  unsigned char* rawData = stbi_load_from_memory(&data[0], data.size(), &result.width, &result.height, &result.channels, 0);
  result.data = std::vector<unsigned char>(rawData, rawData + result.height * result.width * result.channels);
  stbi_image_free(rawData);

  return result;
}
