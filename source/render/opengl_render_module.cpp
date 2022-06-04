#include "opengl_render_module.hpp"

#include <exception>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui_internal.h"

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


void OpenglRenderModule::Update(float delta, GameContext& context)
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
  renderSystem.RenderMap(openglScene_->GetMap(), mapProgram_, context.camera, ratio);

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  auto imguiWindowsIterator = context.scene->GetImguiWindowsIterator();
  for (auto it = imguiWindowsIterator.first; it != imguiWindowsIterator.second; it++)
  {
    it->get()->Render();
  }

  // Render imgui ui
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void OpenglRenderModule::SetContext(GlfwWindow& window)
{
  Enviroment::GetRenderSystem().OnContextChanged();

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;

  ImGui::StyleColorsDark();

  window.InitImgui();
  // GL 3.0 + GLSL 130
  const char* glsl_version = "#version 130";
  ImGui_ImplOpenGL3_Init(glsl_version);

  // Configure
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);

  window.SetFramebufferCallback(
    [this](int width, int height)
    {
      glViewport(0, 0, width, height);
    }
  );

  std::thread::id id = std::this_thread::get_id();
  context_ = std::make_unique<OpenglContext>(window, id);
}

void OpenglRenderModule::InitResources()
{
  if (!IsCorrectThread())
  {
    return;
  }

  // Load map shader program
  std::string vertexCode = blocks::readTextFile(PPCAT(SHADERS_DIR, DEFAULT_VERTEX_SHADER));
  std::string fragmentCode = blocks::readTextFile(PPCAT(SHADERS_DIR, DEFAULT_FRAGMENT_SHADER));
  OpenglShader vertexShader(vertexCode, GL_VERTEX_SHADER);
  OpenglShader fragmentShader(fragmentCode, GL_FRAGMENT_SHADER);
  mapProgram_ = std::make_shared<OpenglProgram>(vertexShader, fragmentShader);

  openglScene_ = std::make_shared<OpenglScene>();
  openglScene_->InitMap();

  ResourceBase& resourceBase = Enviroment::GetResourceSystem();
  std::shared_ptr<BlockSet> blockSet = resourceBase.LoadBlockSet(resourceBase.GetBlockSetNames()->front());
  openglScene_->GetMap()->SetBlockSet(blockSet);
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


bool OpenglRenderModule::IsCorrectThread()
{
  return context_ && context_->threadId_ == std::this_thread::get_id();
}
