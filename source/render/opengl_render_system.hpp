#pragma once

#include <memory>

#include "enviroment_system_interface.hpp"
#include "glew_headers.hpp"
#include "platform/glfw_window.hpp"
#include "opengl_buffer.hpp"
#include "opengl_vertex_array_object.hpp"
#include "opengl_chunk.hpp"
#include "opengl_map.hpp"
#include "opengl_shader.hpp"
#include "opengl_program.hpp"
#include "camera.hpp"
#include "model/chunk.hpp"


class OpenglRenderSystem : EnviromentSystemInterface
{
public:
  OpenglRenderSystem();
  OpenglRenderSystem(const OpenglRenderSystem&) = delete;
  OpenglRenderSystem(OpenglRenderSystem&& other) = delete;
  OpenglRenderSystem& operator=(const OpenglRenderSystem&) = delete;
  OpenglRenderSystem& operator=(OpenglRenderSystem&& other) = delete;
  ~OpenglRenderSystem();

  virtual void Init() override;
  virtual void Deinit() override;
  virtual bool IsInitialized() override;

  void OnContextChanged();

  void Clear(glm::vec4 clearColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  void RenderMap(std::shared_ptr<OpenglMap> map, std::shared_ptr<OpenglProgram> mapProgram, std::shared_ptr<Camera> camera, float ratio);

private:
  void InitResources();
};
