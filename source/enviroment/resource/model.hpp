#pragma once

#include <vector>

#include "mesh.hpp"
#include "material.hpp"


namespace blocks
{
  class Model
  {
  public:
    Model(std::vector<Mesh> meshes, std::vector<Material> materials);

    const std::vector<Mesh>& GetMeshes() const;
    const std::vector<Material>& GetMaterials() const;

  private:
    std::vector<Mesh> meshes_;
    std::vector<Material> materials_;
  };


  inline const std::vector<Mesh>& Model::GetMeshes() const
  {
    return meshes_;
  }

  inline const std::vector<Material>& Model::GetMaterials() const
  {
    return materials_;
  }
}
