#include "opengl_shader.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

#include "io/file_api.hpp"


OpenglShader::OpenglShader(const std::string& shaderCode, GLuint shaderType) : shaderType_(shaderType)
{
  const char* cShaderCode = shaderCode.c_str();
  
  id_ = glCreateShader(shaderType);
  glShaderSource(id_, 1, &cShaderCode, NULL);
  glCompileShader(id_);
  CheckErrors();
}

OpenglShader::OpenglShader(OpenglShader&& other) : id_(other.id_)
{
  other.id_ = 0;
}

OpenglShader& OpenglShader::operator=(OpenglShader&& other)
{
  if (this != &other)
  {
    Release();
    std::swap(id_, other.id_);
  }

  return *this;
}

OpenglShader::~OpenglShader()
{
  Release();
}


GLuint OpenglShader::GetId() const
{
  return id_;
}

GLuint OpenglShader::GetType() const
{
  return shaderType_;
}


void OpenglShader::Release()
{
  glDeleteShader(id_);
  id_ = 0;
}

void OpenglShader::CheckErrors()
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
