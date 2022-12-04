#pragma once

#include <string>

#include "image.hpp"


namespace blocks
{
  class Material
  {
  public:
    inline Material(std::string name, Image colorTexture);

    inline const std::string& GetName() const;
    inline const Image& GetColorTexture() const;

  private:
    std::string name_;
    Image colorTexture_;
  };


  Material::Material(std::string name, Image colorTexture) : name_(name), colorTexture_(colorTexture)
  {
  }


  const std::string& Material::GetName() const
  {
    return name_;
  }

  const Image& Material::GetColorTexture() const
  {
    return colorTexture_;
  }
}
