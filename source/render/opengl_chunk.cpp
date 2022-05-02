#include "opengl_chunk.hpp"


OpenglChunk::OpenglChunk(std::shared_ptr<OpenglBuffer> vbo, std::shared_ptr<OpenglVertexArrayObject> vao, int verticesNumber) 
  : vbo_(vbo), vao_(vao), verticesNumber_(verticesNumber)
{

}

OpenglChunk::~OpenglChunk()
{

}
