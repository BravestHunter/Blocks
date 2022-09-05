#include "shader.hpp"

#include <utility>
#include <iostream>


namespace opengl
{
  Shader::Shader(const std::string& shaderCode, GLuint shaderType) : shaderType_(shaderType)
  {
    const char* cShaderCode = shaderCode.c_str();

    id_ = glCreateShader(shaderType);
    glShaderSource(id_, 1, &cShaderCode, NULL);
    glCompileShader(id_);

    CheckErrors();
  }

  Shader::Shader(Shader&& other) : Object(other.id_)
  {
    other.id_ = 0;
  }

  Shader& Shader::operator=(Shader&& other)
  {
    if (this == &other)
    {
      return *this;
    }

    Release();
    std::swap(id_, other.id_);

    return *this;
  }

  Shader::~Shader()
  {
    Release();
  }


  void Shader::Release()
  {
    if (id_ != 0)
    {
      glDeleteShader(id_);
      id_ = 0;
    }
  }

  void Shader::CheckErrors()
  {
    GLint success;
    GLchar infoLog[1024];

    std::string shaderTypeName;
    switch (shaderType_)
    {
    case GL_VERTEX_SHADER:
      shaderTypeName = "VERTEX SHADER";
      break;
    case GL_FRAGMENT_SHADER:
      shaderTypeName = "FRAGMENT SHADER";
      break;
    default:
      shaderTypeName = "";
      break;
    }

    glGetProgramiv(id_, GL_LINK_STATUS, &success);
    if (!success)
    {
      glGetProgramInfoLog(id_, 1024, NULL, infoLog);
      std::cout << shaderTypeName << " COMPILING ERROR of type:\n" << infoLog << std::endl;
    }
  }
}
