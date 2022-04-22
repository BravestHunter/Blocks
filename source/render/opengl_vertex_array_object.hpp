#pragma once

#include "glew_headers.hpp"


class OpenglVertexArrayObject
{
public:
  OpenglVertexArrayObject();
  ~OpenglVertexArrayObject();

  void Bind();

private:
  unsigned int id_;
};
