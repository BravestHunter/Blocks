#include "game.hpp"

#include <memory>
#include <thread>
#include <string>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui_internal.h"

#include "model/chunk.hpp"
#include "render/opengl_model.hpp"
#include "render/opengl_chunk_model.hpp"
#include "render/opengl_texture_array.hpp"

#include "compile_utils.hpp"
#include "resourceConfig.h"

#include "io/file_api.hpp"

std::shared_ptr<OpenglModel> CreateBlockModel();
std::shared_ptr<Chunk> GenerateChunk();
OpenglRawChunkData GenerateChunkRawData(std::shared_ptr<Chunk> chunk);
std::shared_ptr<OpenglChunkModel> GenerateChunkModel(std::shared_ptr<Chunk> chunk);


Game::Game(int width, int height) : framebufferWidth_(width), framebufferHeight_(height), lastX_(width / 2), lastY_(height / 2)
{
  platform_ = std::make_unique<GlfwPlatform>();
  map_ = std::make_shared<OpenglMapMoodel>();
  camera_ = std::make_unique<Camera>(glm::vec3(-150.0f, -8.0f, 270.0f));

  platform_->Init();
}

Game::~Game()
{
  platform_->Deinit();
}


int Game::Run()
{
  std::thread renderThread(&Game::RunRenderCycle, this);

  // Generate map
  int xRange = 3;
  int yRange = 3;
  for (int x = -xRange; x <= xRange; x++)
  {
    for (int y = -yRange; y <= yRange; y++)
    {
      std::shared_ptr<Chunk> chunk = GenerateChunk();
      OpenglRawChunkData chunkData = GenerateChunkRawData(chunk);
      map_->EnqueueChunk(chunkData, std::make_pair(x, y));

      if (window_ && window_->IsWindowShouldClose())
      {
        break;
      }
    }
  }

  renderThread.join();

  return 0;
}


void Game::RunRenderCycle()
{
  window_ = platform_->CreateWindow(framebufferWidth_, framebufferHeight_, "Title");
  window_->MakeCurrentContext();

  OpenglRenderSystem renderSystem;
  renderSystem.Init();

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;

  ImGui::StyleColorsDark();

  window_->InitImgui();
  // GL 3.0 + GLSL 130
  const char* glsl_version = "#version 130";
  ImGui_ImplOpenGL3_Init(glsl_version);

  bool wireframeMode = false;

  // Set callbacks
  window_->SetFramebufferCallback(
    [this](int width, int height)
    {
      framebufferWidth_ = width;
      framebufferHeight_ = height;

      glViewport(0, 0, width, height);
    }
  );
  window_->SetKeyCallback(
    [this, &renderSystem, &wireframeMode](int keycode, int scancode, int action, int mods)
    {
      if (action != GLFW_PRESS || window_ == nullptr)
        return;

      if (keycode == GLFW_KEY_ESCAPE)
      {
        window_->SetWindowShouldClose(true);
      }

      if (keycode == GLFW_KEY_X)
      {
        wireframeMode = !wireframeMode;
        renderSystem.SetWireframeMode(wireframeMode);
      }

      if (keycode == GLFW_KEY_L)
      {
        SwitchCursorMode();
      }
    }
  );
  window_->SetCursorPositionCallback(
    [this](double xpos, double ypos)
    {
      float xposF = static_cast<float>(xpos);
      float yposF = static_cast<float>(ypos);

      if (firstMouse_)
      {
        lastX_ = xposF;
        lastY_ = yposF;
        firstMouse_ = false;
      }

      float xoffset = xposF - lastX_;
      float yoffset = lastY_ - yposF; // reversed since y-coordinates go from bottom to top

      lastX_ = xposF;
      lastY_ = yposF;

      if (!isCursorEnabled_)
      {
        camera_->ProcessMouseMovement(xoffset, yoffset);
      }
    }
  );
  window_->SetScrollCallback(
    [this](double xoffset, double yoffset)
    {
      camera_->ProcessMouseScroll(static_cast<float>(yoffset));
    }
  );

  std::shared_ptr<OpenglModel> blockModel = CreateBlockModel();

  std::shared_ptr<OpenglTexture> texture = std::make_shared<OpenglTexture>(PPCAT(TEXTURES_DIR, ATLAS_TEXTURE));
  texture->Bind(GL_TEXTURE0);

  std::vector<const char*> paths{ PPCAT(BLOCK_TEXTURES_DIR, "1.png"), PPCAT(BLOCK_TEXTURES_DIR, "2.png"), PPCAT(BLOCK_TEXTURES_DIR, "3.png") };
  OpenglTextureArray texAr(paths, 64, 64);

  bool showImguiWindow = true;

  while (!window_->IsWindowShouldClose())
  {
    glfwPollEvents();

    float currentFrame = static_cast<float>(platform_->GetTime());
    deltaTime_ = currentFrame - lastFrame_;
    lastFrame_ = currentFrame;

    ProcessInput();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(0, 0));
    ImGui::Begin("Info");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Text("Camera position %.2f %.2f %.2f", camera_->GetPosition().x, camera_->GetPosition().y, camera_->GetPosition().z);
    ImGui::Text("Camera direction %.2f %.2f %.2f", camera_->GetForward().x, camera_->GetForward().y, camera_->GetForward().z);
    ImGui::End();

    renderSystem.Clear();

    float framebufferRatio = (float)framebufferWidth_ / (float)framebufferHeight_;

    map_->AddChunksFromQueue();

    renderSystem.RenderMap(map_, camera_.get(), framebufferRatio);

    // Render imgui ui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    window_->SwapBuffers();
  }

  renderSystem.Deinit();
}

void Game::ProcessInput()
{
  if (window_->GetKeyState(GLFW_KEY_W) == GLFW_PRESS)
  {
    camera_->ProcessKeyboard(Camera::FORWARD, deltaTime_);
  }
  if (window_->GetKeyState(GLFW_KEY_S) == GLFW_PRESS)
  {
    camera_->ProcessKeyboard(Camera::BACKWARD, deltaTime_);
  }
  if (window_->GetKeyState(GLFW_KEY_A) == GLFW_PRESS)
  {
    camera_->ProcessKeyboard(Camera::LEFT, deltaTime_);
  }
  if (window_->GetKeyState(GLFW_KEY_D) == GLFW_PRESS)
  {
    camera_->ProcessKeyboard(Camera::RIGHT, deltaTime_);
  }
}

void Game::SwitchCursorMode()
{
  isCursorEnabled_ = !isCursorEnabled_;

  if (isCursorEnabled_)
  {
    window_->SetCursorMode(CursorMode::Normal);
  }
  else
  {
    window_->SetCursorMode(CursorMode::Disabled);
  }
}


std::shared_ptr<OpenglModel> CreateBlockModel()
{
  std::shared_ptr<OpenglBuffer> vbo = std::make_shared<OpenglBuffer>(GL_ARRAY_BUFFER);
  std::shared_ptr<OpenglVertexArrayObject> vao = std::make_shared<OpenglVertexArrayObject>();
  std::shared_ptr<OpenglTexture> texture = std::make_shared<OpenglTexture>(PPCAT(TEXTURES_DIR, BLOCK_TEXTURE));

  float vertices[] = {
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
       0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
  };

  vao->Bind();

  vbo->Bind();
  vbo->SetData(sizeof(vertices), vertices);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  return std::make_shared<OpenglModel>(vbo, vao, texture);
}

std::shared_ptr<Chunk> GenerateChunk()
{
  Chunk* chunk = new Chunk();

  for (int z = 0; z < Chunk::Height; z++)
  {
    for (int y = 0; y < Chunk::Width; y++)
    {
      for (int x = 0; x < Chunk::Length; x++)
      {
        chunk->blocks[x + y * Chunk::Width + z * Chunk::LayerBlocksNumber] = rand() % 4;
      }
    }
  }

  return std::shared_ptr<Chunk>(chunk);
}

struct Vertex
{
  float x;
  float y;
  float z;
  float texU;
  float texV;
  float texI;
};

void AddVertex(Vertex& vertex, float* data, size_t& index)
{
  data[index++] = vertex.x;
  data[index++] = vertex.y;
  data[index++] = vertex.z;
  data[index++] = vertex.texU;
  data[index++] = vertex.texV;
  data[index++] = vertex.texI;
}

OpenglRawChunkData GenerateChunkRawData(std::shared_ptr<Chunk> chunk)
{
  static const size_t BlockVerticesNumber = 4 * 6;
  static const size_t VertexSize = sizeof(float) * 6;
  static const size_t verticesDataSize = Chunk::BlocksNumber * BlockVerticesNumber * VertexSize;

  float* verticesData = new float[verticesDataSize];
  size_t verticesDataIndex = 0;
  size_t verticesNumber = 0;
  for (int z = 0; z < Chunk::Height; z++)
  {
    for (int y = 0; y < Chunk::Width; y++)
    {
      for (int x = 0; x < Chunk::Length; x++)
      {
        size_t blockIndex = x + y * Chunk::Width + z * Chunk::LayerBlocksNumber;

        if (chunk->blocks[blockIndex] == 0)
        {
          continue;
        }

        float fBlock = (float)(chunk->blocks[blockIndex] - 1);

        glm::vec3 position(x, y, z);

        // Check forward face
        if (x == Chunk::Length - 1 || chunk->blocks[blockIndex + 1] == 0)
        {
          // Add forward face

          Vertex v1(x + 1, y + 1, z, 0.0f, 0.0f, fBlock);
          Vertex v2(x + 1, y, z, 1.0f, 0.0f, fBlock);
          Vertex v3(x + 1, y + 1, z + 1, 0.0f, 1.0f, fBlock);
          Vertex v4(x + 1, y, z + 1, 1.0f, 1.0f, fBlock);

          AddVertex(v1, verticesData, verticesDataIndex);
          AddVertex(v2, verticesData, verticesDataIndex);
          AddVertex(v3, verticesData, verticesDataIndex);
          AddVertex(v3, verticesData, verticesDataIndex);
          AddVertex(v2, verticesData, verticesDataIndex);
          AddVertex(v4, verticesData, verticesDataIndex);

          verticesNumber += 6;
        }

        // Check backward face
        if (x == 0 || chunk->blocks[blockIndex - 1] == 0)
        {
          // Add backward face

          Vertex v1(x, y, z, 0.0f, 0.0f, fBlock);
          Vertex v2(x, y + 1, z, 1.0f, 0.0f, fBlock);
          Vertex v3(x, y, z + 1, 0.0f, 1.0f, fBlock);
          Vertex v4(x, y + 1, z + 1, 1.0f, 1.0f, fBlock);

          AddVertex(v1, verticesData, verticesDataIndex);
          AddVertex(v2, verticesData, verticesDataIndex);
          AddVertex(v3, verticesData, verticesDataIndex);
          AddVertex(v3, verticesData, verticesDataIndex);
          AddVertex(v2, verticesData, verticesDataIndex);
          AddVertex(v4, verticesData, verticesDataIndex);

          verticesNumber += 6;
        }

        // Check right face
        if (y == Chunk::Width - 1 || chunk->blocks[blockIndex + Chunk::Length] == 0)
        {
          // Add right face

          Vertex v1(x, y + 1, z, 0.0f, 0.0f, fBlock);
          Vertex v2(x + 1, y + 1, z, 1.0f, 0.0f, fBlock);
          Vertex v3(x, y + 1, z + 1, 0.0f, 1.0f, fBlock);
          Vertex v4(x + 1, y + 1, z + 1, 1.0f, 1.0f, fBlock);

          AddVertex(v1, verticesData, verticesDataIndex);
          AddVertex(v2, verticesData, verticesDataIndex);
          AddVertex(v3, verticesData, verticesDataIndex);
          AddVertex(v3, verticesData, verticesDataIndex);
          AddVertex(v2, verticesData, verticesDataIndex);
          AddVertex(v4, verticesData, verticesDataIndex);

          verticesNumber += 6;
        }

        // Check left face
        if (y == 0 || chunk->blocks[blockIndex - Chunk::Length] == 0)
        {
          // Add left face

          Vertex v1(x + 1, y, z, 0.0f, 0.0f, fBlock);
          Vertex v2(x, y, z, 1.0f, 0.0f, fBlock);
          Vertex v3(x + 1, y, z + 1, 0.0f, 1.0f, fBlock);
          Vertex v4(x, y, z + 1, 1.0f, 1.0f, fBlock);

          AddVertex(v1, verticesData, verticesDataIndex);
          AddVertex(v2, verticesData, verticesDataIndex);
          AddVertex(v3, verticesData, verticesDataIndex);
          AddVertex(v3, verticesData, verticesDataIndex);
          AddVertex(v2, verticesData, verticesDataIndex);
          AddVertex(v4, verticesData, verticesDataIndex);

          verticesNumber += 6;
        }

        // Check upper face
        if (z == Chunk::Height - 1 || chunk->blocks[blockIndex + Chunk::LayerBlocksNumber] == 0)
        {
          // Add upper face

          Vertex v1(x + 1, y, z + 1, 0.0f, 0.0f, fBlock);
          Vertex v2(x, y, z + 1, 1.0f, 0.0f, fBlock);
          Vertex v3(x + 1, y + 1, z + 1, 0.0f, 1.0f, fBlock);
          Vertex v4(x, y + 1, z + 1, 1.0f, 1.0f, fBlock);

          AddVertex(v1, verticesData, verticesDataIndex);
          AddVertex(v2, verticesData, verticesDataIndex);
          AddVertex(v3, verticesData, verticesDataIndex);
          AddVertex(v3, verticesData, verticesDataIndex);
          AddVertex(v2, verticesData, verticesDataIndex);
          AddVertex(v4, verticesData, verticesDataIndex);

          verticesNumber += 6;
        }

        // Check bottom face
        if (z == 0 || chunk->blocks[blockIndex - Chunk::LayerBlocksNumber] == 0)
        {
          // Add bottom face

          Vertex v1(x, y, z, 0.0f, 0.0f, fBlock);
          Vertex v2(x + 1, y, z, 1.0f, 0.0f, fBlock);
          Vertex v3(x, y + 1, z, 0.0f, 1.0f, fBlock);
          Vertex v4(x + 1, y + 1, z, 1.0f, 1.0f, fBlock);

          AddVertex(v1, verticesData, verticesDataIndex);
          AddVertex(v2, verticesData, verticesDataIndex);
          AddVertex(v3, verticesData, verticesDataIndex);
          AddVertex(v3, verticesData, verticesDataIndex);
          AddVertex(v2, verticesData, verticesDataIndex);
          AddVertex(v4, verticesData, verticesDataIndex);

          verticesNumber += 6;
        }
      }
    }
  }

  return OpenglRawChunkData(verticesData, verticesDataIndex + 1, verticesNumber);
}

std::shared_ptr<OpenglChunkModel> GenerateChunkModel(std::shared_ptr<Chunk> chunk)
{
  std::shared_ptr<OpenglBuffer> vbo = std::make_shared<OpenglBuffer>(GL_ARRAY_BUFFER);
  std::shared_ptr<OpenglVertexArrayObject> vao = std::make_shared<OpenglVertexArrayObject>();

  OpenglRawChunkData chunkData = GenerateChunkRawData(chunk);

  vao->Bind();

  vbo->Bind();
  vbo->SetData(sizeof(float) * chunkData.verticesDataLength, chunkData.verticesData);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  return std::make_shared<OpenglChunkModel>(vbo, vao, chunkData.verticesNumber);
}
