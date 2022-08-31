#pragma once

#include <memory>

#include "game_context.hpp"
#include "platform/glfw_window.hpp"
#include "render/glew_headers.hpp"
#include "render/opengl_context.hpp"
#include "render/opengl_program.hpp"
#include "opengl_scene.hpp"
#include "simulation/camera.hpp"
#include "presentation/presentation_context.hpp"


namespace blocks
{
  class OpenglRenderModule
  {
  public:
    OpenglRenderModule();
    OpenglRenderModule(const OpenglRenderModule&) = delete;
    OpenglRenderModule(OpenglRenderModule&& other) = delete;
    OpenglRenderModule& operator=(const OpenglRenderModule&) = delete;
    OpenglRenderModule& operator=(OpenglRenderModule&& other) = delete;
    ~OpenglRenderModule();

    void Update(float delta, PresentationContext& presentationContext, GameContext& gameContext);

    void SetContext(GlfwWindow& window);
    void InitResources(PresentationContext& presentationContext);
    void FreeResources(PresentationContext& presentationContext);

  private:
    bool IsCorrectThread();
    void Clear(glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    void RenderMap(std::shared_ptr<OpenglMap> map, std::shared_ptr<OpenglProgram> mapProgram, std::shared_ptr<Camera> camera, float ratio);

    std::unique_ptr<OpenglContext> context_;
    std::shared_ptr<OpenglProgram> mapProgram_;
  };
}
