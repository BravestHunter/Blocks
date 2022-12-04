#pragma once

#include <string>
#include <vector>


namespace blocks
{
  bool isPathExist(std::string path);
  std::vector<std::string> getFilesInDirectory(std::string path);
  std::vector<std::string> getDirectoriesInDirectory(std::string path);
  std::string GetFileExtension(std::string path);

  void createDirectory(std::string path);

  std::vector<unsigned char> readBinaryFile(std::string path);
  std::string readTextFile(std::string path);

  void saveBinaryFile(std::string path, std::vector<unsigned char> data);
  void saveTextFile(std::string path, std::string str);

  void removePath(std::string path);
}
