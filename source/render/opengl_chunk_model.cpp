#include "opengl_chunk_model.hpp"


OpenglChunkModel::OpenglChunkModel(std::shared_ptr<OpenglBuffer> vbo, std::shared_ptr<OpenglVertexArrayObject> vao, int verticesNumber) 
  : vbo_(vbo), vao_(vao), verticesNumber_(verticesNumber)
{

}

OpenglChunkModel::~OpenglChunkModel()
{

}
