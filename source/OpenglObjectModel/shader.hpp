#pragma once

#include <string>

#include "glew_headers.hpp"


namespace opengl
{
  class Shader
  {
  public:
    Shader(const std::string& shaderCode, GLuint shaderType);
    Shader(const Shader&) = delete;
    Shader(Shader&& other);
    Shader& operator=(const Shader&) = delete;
    Shader& operator=(Shader&& other);
    ~Shader();

    GLuint GetId() const;
    GLuint GetType() const;

  private:
    void Release();
    void CheckErrors();

    GLuint id_;
    GLuint shaderType_;
  };
}
