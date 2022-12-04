#include "mesh.hpp"


namespace blocks
{
  Mesh::Mesh(std::vector<glm::vec3> positions, std::vector<glm::vec2> texCoords, std::vector<std::uint32_t> indices, int materialIndex) 
    : positions_(positions), texCoords_(texCoords), indices_(indices), materialIndex_(materialIndex)
  {
  }
}
