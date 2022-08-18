#pragma once

#include "platform/glfw_window.hpp"
#include "game_context.hpp"
#include "render/opengl_render_module.hpp"


namespace blocks
{
  class PresentationModule
  {
  public:
    PresentationModule();

    void Update(float delta, GameContext& contex);

    void SetContext(GlfwWindow& window);
    void InitResources();
    void FreeResources();

    OpenglRenderModule& GetRenderModule();

  private:
    OpenglRenderModule renderModule_;
  };
}
