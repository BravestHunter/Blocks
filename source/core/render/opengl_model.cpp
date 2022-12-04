#include "opengl_model.hpp"


namespace blocks
{
  struct GlModelVertex
  {
    glm::vec3 positon;
    glm::vec2 texCoords;
  };

  OpenglModel::OpenglModel(std::shared_ptr<opengl::VertexArrayObject> vao, std::shared_ptr<opengl::Buffer> vbo, std::shared_ptr<opengl::Buffer> ebo, size_t indicesCount)
    : vao_(vao), vbo_(vbo), ebo_(ebo), indicesCount_(indicesCount), texture_(nullptr)
  {
  }

  OpenglModel::OpenglModel(const Model& model)
  {
    // Combine all meshes
    std::vector<GlModelVertex> data;
    std::vector<uint32_t> indicesData;
    size_t indicesShift = 0;
    for (const Mesh& mesh : model.GetMeshes())
    {
      const std::vector<glm::vec3>& positions = mesh.GetPositions();
      const std::vector<glm::vec2>& texCoords = mesh.GetTexCoords();

      for (size_t i = 0; i < positions.size(); i++)
      {
        GlModelVertex vertex;
        vertex.positon = positions[i];
        vertex.texCoords = texCoords[i];
        data.push_back(vertex);
      }

      const std::vector<uint32_t>& indices = mesh.GetIndices();
      for (size_t i = 0; i < indices.size(); i++)
      {
        indicesData.push_back(indices[i] + indicesShift);
      }
      indicesShift += positions.size();
    }

    indicesCount_ = indicesData.size();

    vao_ = std::make_shared<opengl::VertexArrayObject>();
    vbo_ = std::make_shared<opengl::Buffer>(GL_ARRAY_BUFFER);
    ebo_ = std::make_shared<opengl::Buffer>(GL_ELEMENT_ARRAY_BUFFER);

    vao_->Bind();

    vbo_->Bind();
    vbo_->SetData(sizeof(GlModelVertex) * data.size(), data.data());

    ebo_->Bind();
    ebo_->SetData(sizeof(uint32_t) * indicesData.size(), indicesData.data());

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    texture_ = std::make_shared<opengl::Texture2D>(model.GetMaterials()[0].GetColorTexture());
  }
}
