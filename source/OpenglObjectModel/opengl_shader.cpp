#include "opengl_shader.hpp"

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

  Shader::Shader(Shader&& other) : id_(other.id_)
  {
    other.id_ = 0;
  }

  Shader& Shader::operator=(Shader&& other)
  {
    if (this != &other)
    {
      Release();
      std::swap(id_, other.id_);
    }

    return *this;
  }

  Shader::~Shader()
  {
    Release();
  }


  GLuint Shader::GetId() const
  {
    return id_;
  }

  GLuint Shader::GetType() const
  {
    return shaderType_;
  }


  void Shader::Release()
  {
    glDeleteShader(id_);
    id_ = 0;
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
    case GL_GEOMETRY_SHADER:
      shaderTypeName = "GEOMETRY SHADER";
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
