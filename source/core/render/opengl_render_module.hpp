#pragma once

#include <memory>

#include "game_module_interface.hpp"
#include "platform/glfw_window.hpp"
#include "render/glew_headers.hpp"
#include "render/opengl_context.hpp"
#include "render/opengl_program.hpp"
#include "opengl_scene.hpp"
#include "camera.hpp"


namespace blocks
{
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
    void Clear(glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    void RenderMap(std::shared_ptr<OpenglMap> map, std::shared_ptr<OpenglProgram> mapProgram, std::shared_ptr<Camera> camera, float ratio);

    std::unique_ptr<OpenglContext> context_;
    std::shared_ptr<OpenglProgram> mapProgram_;
    std::shared_ptr<OpenglScene> openglScene_;
  };
}
