#pragma once

#include <memory>

#include "game_module_interface.hpp"
#include "platform/glfw_window.hpp"
#include "glew_headers.hpp"
#include "opengl_context.hpp"
#include "opengl_program.hpp"
#include "opengl_scene.hpp"
#include "camera.hpp"

class OpenglRenderModule : public GameModuleInterface
{
public:
  OpenglRenderModule();
  OpenglRenderModule(const OpenglRenderModule&) = delete;
  OpenglRenderModule(OpenglRenderModule&& other) = delete;
  OpenglRenderModule& operator=(const OpenglRenderModule&) = delete;
  OpenglRenderModule& operator=(OpenglRenderModule&& other) = delete;
  ~OpenglRenderModule() override;

  virtual void Update(float delta, GameContext& context) override;

  void SetContext(GlfwWindow& window);
  void InitResources();
  void FreeResources();

  std::shared_ptr<OpenglScene> GetOpenglScene();

private:
  bool IsCorrectThread();

  std::unique_ptr<OpenglContext> context_;
  std::shared_ptr<OpenglProgram> mapProgram_;
  std::shared_ptr<OpenglScene> openglScene_;
};
