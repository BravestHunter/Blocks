#include "opengl_model.hpp"


OpenglModel::OpenglModel(std::shared_ptr<OpenglBuffer> vbo, std::shared_ptr<OpenglVertexArrayObject> vao, std::shared_ptr<OpenglTexture> texture) :
  vbo_(vbo), vao_(vao), ebo_(nullptr), texture_(texture)
{

}

OpenglModel::OpenglModel(std::shared_ptr<OpenglBuffer> vbo, std::shared_ptr<OpenglVertexArrayObject> vao, std::shared_ptr<OpenglBuffer> ebo, std::shared_ptr<OpenglTexture> texture) : 
  vbo_(vbo), vao_(vao), ebo_(ebo), texture_(texture)
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
