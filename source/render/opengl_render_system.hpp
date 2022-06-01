#pragma once

#include <memory>

#include "game_system_interface.hpp"
#include "glew_headers.hpp"
#include "opengl_buffer.hpp"
#include "opengl_vertex_array_object.hpp"
#include "opengl_chunk.hpp"
#include "opengl_map.hpp"
#include "opengl_shader.hpp"
#include "opengl_program.hpp"
#include "camera.hpp"
#include "model/chunk.hpp"


class OpenglRenderSystem : public GameSystemInterface
{
public:
  OpenglRenderSystem();
  OpenglRenderSystem(const OpenglRenderSystem&) = delete;
  OpenglRenderSystem(OpenglRenderSystem&& other) = delete;
  OpenglRenderSystem& operator=(const OpenglRenderSystem&) = delete;
  OpenglRenderSystem& operator=(OpenglRenderSystem&& other) = delete;
  ~OpenglRenderSystem() override;

  void StartUp() override;
  void ShutDown() override;
  bool IsWorking() override;

  void StartFrame();
  void FinishFrame();
  void Clear(glm::vec4 clearColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  void RenderMap(std::shared_ptr<OpenglMap> map, Camera& camera, float ratio);

  int GetFrameTrianlgesNumber();

  void SetWireframeMode(bool value);

private:
  bool isWorking_ = false;
  int frameTrianglesNumber_ = 0;
  std::unique_ptr<OpenglProgram> mapProgram_;
};
