#pragma once

void stbi_set_verticall_flip(bool value);
unsigned char* load_image(char const* filename, int* x, int* y, int* comp, int req_comp);
void free_image(unsigned char* data);
