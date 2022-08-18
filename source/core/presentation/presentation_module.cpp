#include "presentation_module.hpp"


namespace blocks
{
  PresentationModule::PresentationModule()
  {
  }


  void PresentationModule::Update(float delta, GameContext& contex)
  {
    renderModule_.Update(delta, contex);
  }


  void PresentationModule::SetContext(GlfwWindow& window)
  {
    renderModule_.SetContext(window);
  }

  void PresentationModule::InitResources()
  {
    renderModule_.InitResources();
  }

  void PresentationModule::FreeResources()
  {
    renderModule_.FreeResources();
  }


  OpenglRenderModule& PresentationModule::GetRenderModule()
  {
    return renderModule_;
  }
}
