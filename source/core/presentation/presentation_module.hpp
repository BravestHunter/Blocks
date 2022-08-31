#pragma once

#include "platform/glfw_window.hpp"
#include "game_context.hpp"
#include "presentation_context.hpp"
#include "map_loading_module.hpp"
#include "render/opengl_render_module.hpp"
#include "simulation/base_model_update_event.hpp"


namespace blocks
{
  class PresentationModule
  {
  public:
    PresentationModule();

    void Update(float delta, GameContext& gameContext);
    void ProcessModelUpdate(BaseModelUpdateEvent* e, GameContext& gameContext);
    void OnSceneChanged(GameContext& gameContext);

    void SetContext(GlfwWindow& window);
    void InitResources();
    void FreeResources();

    MapLoadingModule& GetMapLoadingModule();
    OpenglRenderModule& GetRenderModule();

  private:
    PresentationContext presentationContext_;
    MapLoadingModule mapLoadingModule_;
    OpenglRenderModule renderModule_;
  };
}
