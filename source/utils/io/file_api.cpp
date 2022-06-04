#include "file_api.hpp"

#include <fstream>
#include <sstream>
#include <filesystem>


namespace blocks
{
  bool isPathExist(std::string path)
  {
    return std::filesystem::exists(std::filesystem::path(path));
  }

  std::vector<std::string> getFilesInDirectory(std::string path)
  {
    std::vector<std::string> paths;

    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
      if (!entry.is_directory())
      {
        paths.push_back(entry.path().filename().string());
      }
    }

    return paths;
  }


  void createDirectory(std::string path)
  {
    std::filesystem::create_directory(path);
  }


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


  void saveBinaryFile(std::string path, std::vector<unsigned char> data)
  {
    std::ofstream outputStream;
    std::string result;

    outputStream.open(path, std::ios::out);
    if (outputStream)
    {
      outputStream.write((const char*)(&data[0]), data.size());
    }
    outputStream.close();
  }

  void saveTextFile(std::string path, std::string str)
  {
    std::ofstream outputStream;
    std::string result;

    outputStream.open(path, std::ios::out);
    if (outputStream)
    {
      outputStream << str;
    }
    outputStream.close();
  }


  void removePath(std::string path)
  {
    std::filesystem::remove(std::filesystem::path(path));
  }
}
