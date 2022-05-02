#include "opengl_render_system.hpp"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "compile_utils.hpp"
#include "resourceConfig.h"


OpenglRenderSystem::OpenglRenderSystem()
{

}

OpenglRenderSystem::~OpenglRenderSystem()
{

}


void OpenglRenderSystem::Init()
{
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
  {
    std::cout << "Failed to initialize GLEW" << std::endl;
    return;
  }

  glEnable(GL_DEPTH_TEST);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);

  defaultShader_ = std::make_unique<OpenglShader>(PPCAT(SHADERS_DIR, DEFAULT_VERTEX_SHADER), PPCAT(SHADERS_DIR, DEFAULT_FRAGMENT_SHADER));
}

void OpenglRenderSystem::Deinit()
{
}


void OpenglRenderSystem::Clear(glm::vec4 clearColor)
{
  glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenglRenderSystem::RenderModel(std::shared_ptr<OpenglModel> model, glm::mat4 modelTransform, Camera* camera, float ratio)
{
  defaultShader_->Use();
  model->GetTexture()->Bind(GL_TEXTURE0);
  defaultShader_->SetInt("texture", 0);

  glm::mat4 projection = glm::perspective(glm::radians(camera->GetZoom()), ratio, 0.1f, 100.0f);
  glm::mat4 view = camera->GetViewMatrix();
  glm::mat4 mvp = projection * view * modelTransform;
  defaultShader_->SetMat4("MVP", mvp);

  glDrawArrays(GL_TRIANGLES, 0, 36);
}

void OpenglRenderSystem::RenderChunk(std::shared_ptr<OpenglChunk> chunk, Camera* camera, float ratio)
{
  defaultShader_->Use();
  defaultShader_->SetInt("texture", 0);

  glm::mat4 projection = glm::perspective(glm::radians(camera->GetZoom()), ratio, 0.1f, 100.0f);
  glm::mat4 view = camera->GetViewMatrix();
  glm::mat4 modelTransform(1.0f);
  glm::mat4 mvp = projection * view * modelTransform;
  defaultShader_->SetMat4("MVP", mvp);

  glDrawArrays(GL_TRIANGLES, 0, chunk->verticesNumber_);
}

void OpenglRenderSystem::RenderMap(std::shared_ptr<OpenglMap> map, Camera* camera, float ratio)
{
  defaultShader_->Use();
  defaultShader_->SetInt("texture", 0);

  glm::mat4 projection = glm::perspective(glm::radians(camera->GetZoom()), ratio, 0.1f, 1000.0f);
  glm::mat4 view = camera->GetViewMatrix();

  for (auto pair : map->chunks_)
  {
    std::pair<int, int> coords = pair.first;
    std::shared_ptr<OpenglChunk> chunk = pair.second;

    glm::vec3 chunkOffset(coords.first * (int)Chunk::Length, coords.second * (int)Chunk::Width, 0.0f);
    glm::mat4 modelTransform = glm::translate(glm::mat4(1.0f), chunkOffset);
    glm::mat4 mvp = projection * view * modelTransform;
    defaultShader_->SetMat4("MVP", mvp);

    chunk->vao_->Bind();
    glDrawArrays(GL_TRIANGLES, 0, chunk->verticesNumber_);
  }
}

void OpenglRenderSystem::SetWireframeMode(bool value)
{
  if (value)
  {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }
  else
  {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
}
