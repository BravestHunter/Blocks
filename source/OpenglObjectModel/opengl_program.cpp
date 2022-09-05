#include "opengl_program.hpp"

#include <iostream>


namespace opengl
{
  ShaderProgram::ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader)
  {
    id_ = glCreateProgram();
    glAttachShader(id_, vertexShader.GetId());
    glAttachShader(id_, fragmentShader.GetId());
    glLinkProgram(id_);
    CheckErrors();
  }

  ShaderProgram::ShaderProgram(ShaderProgram&& other) : id_(other.id_)
  {
    other.id_ = 0;
  }

  ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other)
  {
    if (this != &other)
    {
      Release();
      std::swap(id_, other.id_);
    }

    return *this;
  }

  ShaderProgram::~ShaderProgram()
  {
    Release();
  }


  void ShaderProgram::Setup()
  {
    glUseProgram(id_);
  }


  void ShaderProgram::SetBool(const std::string& name, bool value) const
  {
    glUniform1i(glGetUniformLocation(id_, name.c_str()), (int)value);
  }

  void ShaderProgram::SetInt(const std::string& name, int value) const
  {
    glUniform1i(glGetUniformLocation(id_, name.c_str()), value);
  }

  void ShaderProgram::SetFloat(const std::string& name, float value) const
  {
    glUniform1f(glGetUniformLocation(id_, name.c_str()), value);
  }

  void ShaderProgram::SetVec2(const std::string& name, const glm::vec2& value) const
  {
    glUniform2fv(glGetUniformLocation(id_, name.c_str()), 1, &value[0]);
  }

  void ShaderProgram::SetVec2(const std::string& name, float x, float y) const
  {
    glUniform2f(glGetUniformLocation(id_, name.c_str()), x, y);
  }

  void ShaderProgram::SetVec3(const std::string& name, const glm::vec3& value) const
  {
    glUniform3fv(glGetUniformLocation(id_, name.c_str()), 1, &value[0]);
  }

  void ShaderProgram::SetVec3(const std::string& name, float x, float y, float z) const
  {
    glUniform3f(glGetUniformLocation(id_, name.c_str()), x, y, z);
  }

  void ShaderProgram::SetVec4(const std::string& name, const glm::vec4& value) const
  {
    glUniform4fv(glGetUniformLocation(id_, name.c_str()), 1, &value[0]);
  }

  void ShaderProgram::SetVec4(const std::string& name, float x, float y, float z, float w) const
  {
    glUniform4f(glGetUniformLocation(id_, name.c_str()), x, y, z, w);
  }

  void ShaderProgram::SetIVec2(const std::string& name, int x, int y) const
  {
    glUniform2i(glGetUniformLocation(id_, name.c_str()), x, y);
  }

  void ShaderProgram::SetMat2(const std::string& name, const glm::mat2& mat) const
  {
    glUniformMatrix2fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
  }
  void ShaderProgram::SetMat3(const std::string& name, const glm::mat3& mat) const
  {
    glUniformMatrix3fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
  }
  void ShaderProgram::SetMat4(const std::string& name, const glm::mat4& mat) const
  {
    glUniformMatrix4fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
  }


  void ShaderProgram::Release()
  {
    glDeleteProgram(id_);
    id_ = 0;
  }

  void ShaderProgram::CheckErrors()
  {
    GLint success;
    GLchar infoLog[1024];

    glGetProgramiv(id_, GL_LINK_STATUS, &success);
    if (!success)
    {
      glGetProgramInfoLog(id_, 1024, NULL, infoLog);
      std::cout << "PROGRAM LINKING ERROR:\n" << infoLog << std::endl;
    }
  }
}
