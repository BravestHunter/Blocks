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


namespace blocks
{
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

    Clear();

    glm::ivec2 windowSize = context_->window_.GetSize();
    float ratio = (float)windowSize.x / (float)windowSize.y;
    RenderMap(openglScene_->GetMap(), mapProgram_, context.camera, ratio);

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
    GLenum initResult = glewInit();
    if (initResult != GLEW_OK)
    {
      throw std::exception("Failed to initialize GLEW");
    }

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

  void OpenglRenderModule::Clear(glm::vec4 color)
  {
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void OpenglRenderModule::RenderMap(std::shared_ptr<OpenglMap> map, std::shared_ptr<OpenglProgram> mapProgram, std::shared_ptr<Camera> camera, float ratio)
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
}
