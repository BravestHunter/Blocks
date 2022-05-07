#pragma once

#include <string>
#include <vector>

#include "image.hpp"


bool isPathExist(std::string path);
std::vector<std::string> getFilesInDirectory(std::string path);

void createDirectory(std::string path);

std::vector<unsigned char> readBinaryFile(std::string path);
std::string readTextFile(std::string path);
Image readImage(std::string path);

void saveBinaryFile(std::string path, std::vector<unsigned char> data);
void saveTextFile(std::string path, std::string str);

void removePath(std::string path);
