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
  glm::vec4 skyColor = glm::vec4(0.5f, 0.5f, 0.92f, 1.0f);

  OpenglRenderModule::OpenglRenderModule()
  {
  }

  OpenglRenderModule::~OpenglRenderModule()
  {

  }


  void OpenglRenderModule::Update(float delta, PresentationContext& presentationContext, GameContext& gameContext)
  {
    if (!IsCorrectThread())
    {
      return;
    }

    Clear(skyColor);

    glm::ivec2 windowSize = context_->window_.GetSize();
    if (windowSize != context_->viewportSize)
    {
      SetViewportSize(windowSize);
    }

    if (gameContext.scene->ContainsWorld())
    {
      float ratio = (float)windowSize.x / (float)windowSize.y;
      RenderChunks(presentationContext.openglScene->GetMap(), gameContext.camera, ratio);

      RenderHUD();
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    auto imguiWindowsIterator = gameContext.scene->GetImguiWindowsIterator();
    for (auto it = imguiWindowsIterator.first; it != imguiWindowsIterator.second; it++)
    {
      it->get()->Render(context_.get());
    }

    // Render imgui ui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  }


  void OpenglRenderModule::SetViewportSize(glm::ivec2 size)
  {
    if (!IsCorrectThread())
    {
      return;
    }

    glViewport(0, 0, size.x, size.y);
    context_->viewportSize = size;
  }


  void OpenglRenderModule::SetContext(GlfwWindow& window)
  {
    GLenum initResult = glewInit();
    if (initResult != GLEW_OK)
    {
      throw std::exception("Failed to initialize GLEW");
    }

    SetViewportSize(window.GetSize());

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
    glCullFace(GL_BACK);

    window.SetFramebufferCallback(
      [this](int width, int height)
      {
        glViewport(0, 0, width, height);
      }
    );

    std::thread::id id = std::this_thread::get_id();
    context_ = std::make_unique<OpenglContext>(window, id);
  }

  void OpenglRenderModule::InitResources(PresentationContext& presentationContext)
  {
    if (!IsCorrectThread())
    {
      return;
    }

    presentationContext.openglScene = std::make_shared<OpenglScene>();
    presentationContext.openglScene->InitMap();

    ResourceBase& resourceBase = Environment::GetResource();
    std::shared_ptr<BlockSet> blockSet = resourceBase.LoadBlockSet(resourceBase.GetBlockSetNames()->front());
    presentationContext.openglScene->GetMap()->SetBlockSet(blockSet);

    // Load chunk shader program
    std::string vertexCode = blocks::readTextFile(PPCAT(SHADERS_DIR, CHUNK_VERTEX_SHADER));
    std::string fragmentCode = blocks::readTextFile(PPCAT(SHADERS_DIR, CHUNK_FRAGMENT_SHADER));
    opengl::Shader vertexShader(vertexCode, GL_VERTEX_SHADER);
    opengl::Shader fragmentShader(fragmentCode, GL_FRAGMENT_SHADER);
    chunkProgram_ = std::make_shared<opengl::ShaderProgram>(vertexShader, fragmentShader);

    // Load sprite shader program
    vertexCode = blocks::readTextFile(PPCAT(SHADERS_DIR, SPRITE_VERTEX_SHADER));
    fragmentCode = blocks::readTextFile(PPCAT(SHADERS_DIR, SPRITE_FRAGMENT_SHADER));
    vertexShader = opengl::Shader(vertexCode, GL_VERTEX_SHADER);
    fragmentShader = opengl::Shader(fragmentCode, GL_FRAGMENT_SHADER);
    spriteProgram_ = std::make_shared<opengl::ShaderProgram>(vertexShader, fragmentShader);

    // Load crosshair
    Image crosshairImage = resourceBase.ReadImage("resources/textures/crosshair.png");
    crosshairSprite_ = std::make_unique<OpenglSprite>(crosshairImage);
  }

  void OpenglRenderModule::FreeResources(PresentationContext& presentationContext)
  {
    chunkProgram_.reset();
    spriteProgram_.reset();
    presentationContext.openglScene.reset();
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

  void OpenglRenderModule::RenderChunks(std::shared_ptr<OpenglMap> map, std::shared_ptr<Camera> camera, float ratio)
  {
    chunkProgram_->Setup();

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), ratio, 0.1f, 1000.0f);
    glm::mat4 view = camera->GetViewMatrix();

    for (auto pair : map->chunks_)
    {
      ChunkPosition position = pair.first;
      std::shared_ptr<OpenglChunk> chunk = pair.second;

      glm::vec3 chunkOffset(position.x * (int)Chunk::Length, position.y * (int)Chunk::Width, 0.0f);
      glm::mat4 modelTransform = glm::translate(glm::mat4(1.0f), chunkOffset);
      glm::mat4 mvp = projection * view * modelTransform;
      chunkProgram_->SetMat4("MVP", mvp);

      chunk->vao_->Bind();
      glDrawArrays(GL_TRIANGLES, 0, chunk->verticesNumber_);
    }
  }

  void OpenglRenderModule::RenderHUD()
  {
    spriteProgram_->Setup();

    glm::vec2 crosshairSize(32.0f, 32.0f);

    glm::ivec2 viewportSize = context_->viewportSize;
    glm::mat4 modelTransform = glm::mat4(1.0f);
    modelTransform = glm::translate(modelTransform, glm::vec3(viewportSize.x / 2 - crosshairSize.x / 2, viewportSize.y / 2 - crosshairSize.y / 2, 0.0f));
    modelTransform = glm::scale(modelTransform, glm::vec3(crosshairSize.x, crosshairSize.y, 1.0f));
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(viewportSize.x), static_cast<float>(viewportSize.y), 0.0f, -1.0f, 1.0f);
    glm::mat4 mvp = projection * modelTransform;
    spriteProgram_->SetMat4("MVP", mvp);

    crosshairSprite_->GetVao()->Bind();
    crosshairSprite_->GetTexture()->Bind(0);

    glDrawArrays(GL_TRIANGLES, 0, 6);
  }
}
