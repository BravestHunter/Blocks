#include "opengl_render_system.hpp"

#include <exception>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "compile_utils.hpp"
#include "resourceConfig.h"
#include "io/file_api.hpp"


OpenglRenderSystem::OpenglRenderSystem()
{

}

OpenglRenderSystem::~OpenglRenderSystem()
{

}


void OpenglRenderSystem::Init()
{
  // Nothing to do here
}

void OpenglRenderSystem::Deinit()
{
  // Nothing to do here
}

bool OpenglRenderSystem::IsInitialized()
{
  return true;
}


void OpenglRenderSystem::OnContextChanged()
{
  GLenum initResult = glewInit();
  if (initResult != GLEW_OK)
  {
    throw std::exception("Failed to initialize GLEW");
  }
}


void OpenglRenderSystem::Clear(glm::vec4 clearColor)
{
  glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenglRenderSystem::RenderMap(std::shared_ptr<OpenglMap> map, std::shared_ptr<OpenglProgram> mapProgram, std::shared_ptr<Camera> camera, float ratio)
{
  mapProgram->Setup();
  mapProgram->SetInt("texture", 0);

  glm::mat4 projection = glm::perspective(glm::radians(camera->GetZoom()), ratio, 0.1f, 1000.0f);
  glm::mat4 view = camera->GetViewMatrix();

  for (auto pair : map->chunks_)
  {
    std::pair<int, int> coords = pair.first;
    std::shared_ptr<OpenglChunk> chunk = pair.second;

    glm::vec3 chunkOffset(coords.first * (int)Chunk::Length, coords.second * (int)Chunk::Width, 0.0f);
    glm::mat4 modelTransform = glm::translate(glm::mat4(1.0f), chunkOffset);
    glm::mat4 mvp = projection * view * modelTransform;
    mapProgram->SetMat4("MVP", mvp);

    chunk->vao_->Bind();
    glDrawArrays(GL_TRIANGLES, 0, chunk->verticesNumber_);
  }
}


void OpenglRenderSystem::InitResources()
{

}
