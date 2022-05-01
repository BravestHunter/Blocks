#include "opengl_shader.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

#include "io/file_api.hpp"


OpenglShader::OpenglShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
  std::string vertexCode = readTextFile(vertexPath);
  std::string fragmentCode = readTextFile(fragmentPath);
  std::string geometryCode;

  if (geometryPath != nullptr)
  {
    geometryCode = readTextFile(geometryPath);
  }

  const char* vShaderCode = vertexCode.c_str();
  const char* fShaderCode = fragmentCode.c_str();
  
  unsigned int vertex, fragment;
  
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, NULL);
  glCompileShader(vertex);
  CheckCompileErrors(vertex, "VERTEX");
  
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, NULL);
  glCompileShader(fragment);
  CheckCompileErrors(fragment, "FRAGMENT");
  
  unsigned int geometry;
  if (geometryPath != nullptr)
  {
    const char* gShaderCode = geometryCode.c_str();
    geometry = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometry, 1, &gShaderCode, NULL);
    glCompileShader(geometry);
    CheckCompileErrors(geometry, "GEOMETRY");
  }
  
  id_ = glCreateProgram();
  glAttachShader(id_, vertex);
  glAttachShader(id_, fragment);
  if (geometryPath != nullptr)
    glAttachShader(id_, geometry);
  glLinkProgram(id_);
  CheckCompileErrors(id_, "PROGRAM");
  
  glDeleteShader(vertex);
  glDeleteShader(fragment);
  if (geometryPath != nullptr)
    glDeleteShader(geometry);

}

OpenglShader::~OpenglShader()
{
  glDeleteProgram(id_);
}


void OpenglShader::Use()
{
  glUseProgram(id_);
}

void OpenglShader::SetBool(const std::string& name, bool value) const
{
  glUniform1i(glGetUniformLocation(id_, name.c_str()), (int)value);
}

void OpenglShader::SetInt(const std::string& name, int value) const
{
  glUniform1i(glGetUniformLocation(id_, name.c_str()), value);
}

void OpenglShader::SetFloat(const std::string& name, float value) const
{
  glUniform1f(glGetUniformLocation(id_, name.c_str()), value);
}


void OpenglShader::SetVec2(const std::string& name, const glm::vec2& value) const
{
  glUniform2fv(glGetUniformLocation(id_, name.c_str()), 1, &value[0]);
}

void OpenglShader::SetVec2(const std::string& name, float x, float y) const
{
  glUniform2f(glGetUniformLocation(id_, name.c_str()), x, y);
}

void OpenglShader::SetVec3(const std::string& name, const glm::vec3& value) const
{
  glUniform3fv(glGetUniformLocation(id_, name.c_str()), 1, &value[0]);
}

void OpenglShader::SetVec3(const std::string& name, float x, float y, float z) const
{
  glUniform3f(glGetUniformLocation(id_, name.c_str()), x, y, z);
}

void OpenglShader::SetVec4(const std::string& name, const glm::vec4& value) const
{
  glUniform4fv(glGetUniformLocation(id_, name.c_str()), 1, &value[0]);
}

void OpenglShader::SetVec4(const std::string& name, float x, float y, float z, float w) const
{
  glUniform4f(glGetUniformLocation(id_, name.c_str()), x, y, z, w);
}


void OpenglShader::SetIVec2(const std::string& name, int x, int y) const
{
  glUniform2i(glGetUniformLocation(id_, name.c_str()), x, y);
}


void OpenglShader::SetMat2(const std::string& name, const glm::mat2& mat) const
{
  glUniformMatrix2fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void OpenglShader::SetMat3(const std::string& name, const glm::mat3& mat) const
{
  glUniformMatrix3fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void OpenglShader::SetMat4(const std::string& name, const glm::mat4& mat) const
{
  glUniformMatrix4fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}


void OpenglShader::CheckCompileErrors(GLuint shader, std::string type)
{
  GLint success;
  GLchar infoLog[1024];
  if (type != "PROGRAM")
  {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(shader, 1024, NULL, infoLog);
      std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
    }
  }
  else
  {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success)
    {
      glGetProgramInfoLog(shader, 1024, NULL, infoLog);
      std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
    }
  }
}
