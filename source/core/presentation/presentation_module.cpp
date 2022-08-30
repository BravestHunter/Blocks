#include "presentation_module.hpp"


namespace blocks
{
  PresentationModule::PresentationModule()
  {
    mapLoadingModule_.SetRenderModule(&renderModule_);
  }


  void PresentationModule::Update(float delta, GameContext& context)
  {
    mapLoadingModule_.Update(delta, context);
    renderModule_.Update(delta, context);
  }

  void PresentationModule::ProcessModelUpdate(BaseModelUpdateEvent* e, GameContext& context)
  {
    mapLoadingModule_.ProcessModelUpdate(e, context);
  }

  void PresentationModule::OnSceneChanged(GameContext& context)
  {
    mapLoadingModule_.OnSceneChanged(context);
  }


  void PresentationModule::SetContext(GlfwWindow& window)
  {
    renderModule_.SetContext(window);
  }

  void PresentationModule::InitResources()
  {
    renderModule_.InitResources();
    mapLoadingModule_.InitResources();
  }

  void PresentationModule::FreeResources()
  {
    renderModule_.FreeResources();
  }


  MapLoadingModule& PresentationModule::GetMapLoadingModule()
  {
    return mapLoadingModule_;
  }

  OpenglRenderModule& PresentationModule::GetRenderModule()
  {
    return renderModule_;
  }
}
