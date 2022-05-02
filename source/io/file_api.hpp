#pragma once

#include <string>
#include <vector>

#include "image.hpp"


std::vector<unsigned char> readBinaryFile(std::string path);
std::string readTextFile(std::string path);
Image readImage(std::string path);
