#include "opengl_render_system.hpp"

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
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
  {
    std::cout << "Failed to initialize GLEW" << std::endl;
    return;
  }

  glEnable(GL_DEPTH_TEST);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);

  std::string vertexCode = readTextFile(PPCAT(SHADERS_DIR, DEFAULT_VERTEX_SHADER));
  std::string fragmentCode = readTextFile(PPCAT(SHADERS_DIR, DEFAULT_FRAGMENT_SHADER));
  OpenglShader vertexShader(vertexCode, GL_VERTEX_SHADER);
  OpenglShader fragmentShader(fragmentCode, GL_FRAGMENT_SHADER);
  mapProgram_ = std::make_unique<OpenglProgram>(vertexShader, fragmentShader);
}

void OpenglRenderSystem::Deinit()
{
}


void OpenglRenderSystem::StartFrame()
{
  frameTrianglesNumber_ = 0;
}

void OpenglRenderSystem::FinishFrame()
{

}

void OpenglRenderSystem::Clear(glm::vec4 clearColor)
{
  glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenglRenderSystem::RenderMap(std::shared_ptr<OpenglMap> map, Camera* camera, float ratio)
{
  mapProgram_->Setup();
  mapProgram_->SetInt("texture", 0);

  glm::mat4 projection = glm::perspective(glm::radians(camera->GetZoom()), ratio, 0.1f, 1000.0f);
  glm::mat4 view = camera->GetViewMatrix();

  for (auto pair : map->chunks_)
  {
    std::pair<int, int> coords = pair.first;
    std::shared_ptr<OpenglChunk> chunk = pair.second;

    glm::vec3 chunkOffset(coords.first * (int)Chunk::Length, coords.second * (int)Chunk::Width, 0.0f);
    glm::mat4 modelTransform = glm::translate(glm::mat4(1.0f), chunkOffset);
    glm::mat4 mvp = projection * view * modelTransform;
    mapProgram_->SetMat4("MVP", mvp);

    chunk->vao_->Bind();
    glDrawArrays(GL_TRIANGLES, 0, chunk->verticesNumber_);

    frameTrianglesNumber_ += chunk->verticesNumber_ / 3;
  }
}


int OpenglRenderSystem::GetFrameTrianlgesNumber()
{
  return frameTrianglesNumber_;
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
