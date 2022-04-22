#include "opengl_model.hpp"


OpenglModel::OpenglModel(std::shared_ptr<OpenglBuffer> vbo, std::shared_ptr<OpenglVertexArrayObject> vao, std::shared_ptr<OpenglTexture> texture) :
  vbo_(vbo), vao_(vao), texture_(texture)
{

}

OpenglModel::~OpenglModel()
{

}


std::shared_ptr<OpenglBuffer> OpenglModel::GetVBO()
{
  return vbo_;
}

std::shared_ptr<OpenglVertexArrayObject> OpenglModel::GetVAO()
{
  return vao_;
}

std::shared_ptr<OpenglTexture> OpenglModel::GetTexture()
{
  return texture_;
}
