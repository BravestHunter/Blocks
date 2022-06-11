#pragma once

#include <string>

#include "glew_headers.hpp"


namespace blocks
{
  class OpenglShader
  {
  public:
    OpenglShader(const std::string& shaderCode, GLuint shaderType);
    OpenglShader(const OpenglShader&) = delete;
    OpenglShader(OpenglShader&& other);
    OpenglShader& operator=(const OpenglShader&) = delete;
    OpenglShader& operator=(OpenglShader&& other);
    ~OpenglShader();

    GLuint GetId() const;
    GLuint GetType() const;

  private:
    void Release();
    void CheckErrors();

    GLuint id_;
    GLuint shaderType_;
  };
}
