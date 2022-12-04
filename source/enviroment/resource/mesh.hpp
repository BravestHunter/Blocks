#pragma once

#include <vector>
#include <cstdint>

#include "glm/glm.hpp"


namespace blocks
{
  class Mesh
  {
  public:
    Mesh(std::vector<glm::vec3> positions, std::vector<glm::vec2> texCoords, std::vector<std::uint32_t> indices, int materialIndex);

    const std::vector<glm::vec3>& GetPositions() const;
    const std::vector<glm::vec2>& GetTexCoords() const;
    const std::vector<std::uint32_t>& GetIndices() const;
    const int GetMaterialIndex() const;

  private:
    std::vector<glm::vec3> positions_;
    std::vector<glm::vec2> texCoords_;
    std::vector<std::uint32_t> indices_;
    int materialIndex_;
  };


  inline const std::vector<glm::vec3>& Mesh::GetPositions() const
  {
    return positions_;
  }

  inline const std::vector<glm::vec2>& Mesh::GetTexCoords() const
  {
    return texCoords_;
  }

  inline const std::vector<std::uint32_t>& Mesh::GetIndices() const
  {
    return indices_;
  }

  inline const int Mesh::GetMaterialIndex() const
  {
    return materialIndex_;
  }
}
