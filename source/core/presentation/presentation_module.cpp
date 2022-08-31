#include "presentation_module.hpp"

#include "environment.hpp"


namespace blocks
{
  PresentationModule::PresentationModule()
  {
  }


  void PresentationModule::Update(float delta, GameContext& gameContext)
  {
    mapLoadingModule_.Update(delta, presentationContext_, gameContext);
    renderModule_.Update(delta, presentationContext_, gameContext);
  }

  void PresentationModule::ProcessModelUpdate(BaseModelUpdateEvent* e, GameContext& gameContext)
  {
    mapLoadingModule_.ProcessModelUpdate(e, presentationContext_, gameContext);
  }

  void PresentationModule::OnSceneChanged(GameContext& gameContext)
  {
    mapLoadingModule_.OnSceneChanged(presentationContext_, gameContext);
  }


  void PresentationModule::SetContext(GlfwWindow& window)
  {
    renderModule_.SetContext(window);
  }

  void PresentationModule::InitResources()
  {
    ResourceBase& resourceBase = Environment::GetResource();
    presentationContext_.blockSet = resourceBase.LoadBlockSet(resourceBase.GetBlockSetNames()->front());

    renderModule_.InitResources(presentationContext_);
  }

  void PresentationModule::FreeResources()
  {
    renderModule_.FreeResources(presentationContext_);
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
