#pragma once

#include <string>

#include "glew_headers.hpp"
#include "object.hpp"


namespace opengl
{
  class Shader : public Object
  {
  public:
    Shader(const std::string& shaderCode, GLuint shaderType);
    Shader(const Shader&) = delete;
    Shader(Shader&& other);
    Shader& operator=(const Shader&) = delete;
    Shader& operator=(Shader&& other);
    virtual ~Shader() override;

    inline GLuint GetType() const;

  private:
    void Release();
    void CheckErrors();

    GLuint shaderType_;
  };


  GLuint Shader::GetType() const
  {
    return shaderType_;
  }
}
