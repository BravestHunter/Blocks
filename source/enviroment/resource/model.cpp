#include "model.hpp"


namespace blocks
{
  Model::Model(std::vector<Mesh> meshes, std::vector<Material> materials) : meshes_(meshes), materials_(materials)
  {
  }
}
