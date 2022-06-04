#pragma once

#include <glm/glm.hpp>

#include "glew_headers.hpp"
#include "opengl_shader.hpp"


namespace blocks
{
  class OpenglProgram
  {
  public:
    OpenglProgram(const OpenglShader& vertexShader, const OpenglShader& fragmentShader);
    OpenglProgram(const OpenglProgram&) = delete;
    OpenglProgram(OpenglProgram&& other);
    OpenglProgram& operator=(const OpenglProgram&) = delete;
    OpenglProgram& operator=(OpenglProgram&& other);
    ~OpenglProgram();

    void Setup();

    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetVec2(const std::string& name, const glm::vec2& value) const;
    void SetVec2(const std::string& name, float x, float y) const;
    void SetVec3(const std::string& name, const glm::vec3& value) const;
    void SetVec3(const std::string& name, float x, float y, float z) const;
    void SetVec4(const std::string& name, const glm::vec4& value) const;
    void SetVec4(const std::string& name, float x, float y, float z, float w) const;
    void SetIVec2(const std::string& name, int x, int y) const;
    void SetMat2(const std::string& name, const glm::mat2& mat) const;
    void SetMat3(const std::string& name, const glm::mat3& mat) const;
    void SetMat4(const std::string& name, const glm::mat4& mat) const;

  private:
    void Release();
    void CheckErrors();

    GLuint id_;
  };
}
