#include "opengl_render_module.hpp"

#include <exception>

#include "resourceConfig.h"

#include "compile_utils.hpp"
#include "io/file_api.hpp"
#include "environment.hpp"


OpenglRenderModule::OpenglRenderModule()
{
}

OpenglRenderModule::~OpenglRenderModule()
{

}


void OpenglRenderModule::Update(float delta)
{
  if (!IsCorrectThread())
  {
    return;
  }

  openglScene_->GetMap()->ProcessQueues();

  OpenglRenderSystem& renderSystem = Enviroment::GetRenderSystem();

  renderSystem.Clear();

  glm::ivec2 windowSize = context_->window_.GetSize();
  float ratio = (float)windowSize.x / (float)windowSize.y;
  renderSystem.RenderMap(openglScene_->GetMap(), mapProgram_, camera_, ratio);
}


void OpenglRenderModule::SetContext(GlfwWindow& context)
{
  Enviroment::GetRenderSystem().OnContextChanged();

  // Configure
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);

  std::thread::id id = std::this_thread::get_id();
  context_ = std::make_unique<OpenglContext>(context, id);
}

void OpenglRenderModule::InitResources()
{
  if (!IsCorrectThread())
  {
    return;
  }

  // Load map shader program
  std::string vertexCode = readTextFile(PPCAT(SHADERS_DIR, DEFAULT_VERTEX_SHADER));
  std::string fragmentCode = readTextFile(PPCAT(SHADERS_DIR, DEFAULT_FRAGMENT_SHADER));
  OpenglShader vertexShader(vertexCode, GL_VERTEX_SHADER);
  OpenglShader fragmentShader(fragmentCode, GL_FRAGMENT_SHADER);
  mapProgram_ = std::make_shared<OpenglProgram>(vertexShader, fragmentShader);

  openglScene_ = std::make_shared<OpenglScene>();
  openglScene_->InitMap();
}

void OpenglRenderModule::FreeResources()
{
  mapProgram_.reset();
  openglScene_.reset();
}


std::shared_ptr<OpenglScene> OpenglRenderModule::GetOpenglScene()
{
  if (!openglScene_)
  {
    throw new std::exception("Opengl scene wasn't set");
  }

  return openglScene_;
}


void OpenglRenderModule::SetCamera(std::shared_ptr<Camera> camera)
{
  camera_ = camera;
}


bool OpenglRenderModule::IsCorrectThread()
{
  return context_ && context_->threadId_ == std::this_thread::get_id();
}
