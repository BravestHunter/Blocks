#pragma once

#include "platform/glfw_window.hpp"
#include "game_context.hpp"
#include "map_loading_module.hpp"
#include "render/opengl_render_module.hpp"
#include "simulation/base_model_update_event.hpp"


namespace blocks
{
  class PresentationModule
  {
  public:
    PresentationModule();

    void Update(float delta, GameContext& context);
    void ProcessModelUpdate(BaseModelUpdateEvent* e, GameContext& contex);
    void OnSceneChanged(GameContext& context);

    void SetContext(GlfwWindow& window);
    void InitResources();
    void FreeResources();

    MapLoadingModule& GetMapLoadingModule();
    OpenglRenderModule& GetRenderModule();

  private:
    MapLoadingModule mapLoadingModule_;
    OpenglRenderModule renderModule_;
  };
}
