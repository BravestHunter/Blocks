#pragma once

#include "glew_headers.hpp"


namespace opengl
{
  class Object
  {
  protected:
    Object() : id_(0) {}
    Object(GLuint id) : id_(id) {}

  public:
    Object(const Object&) = delete;
    Object(Object&& other) = delete;
    Object& operator=(const Object&) = delete;
    Object& operator=(Object&& other) = delete;
    virtual ~Object() {}

    inline GLuint GetId() const;

  protected:
    GLuint id_;
  };


  GLuint Object::GetId() const
  {
    return id_;
  }
}
