#include "stb_loader.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"


void stbi_set_verticall_flip(bool value)
{
  stbi_set_flip_vertically_on_load(value);
}

unsigned char* load_image(char const* filename, int* x, int* y, int* comp, int req_comp)
{
  return stbi_load(filename, x, y, comp, req_comp);
}

void free_image(unsigned char* data)
{
  stbi_image_free(data);
}
