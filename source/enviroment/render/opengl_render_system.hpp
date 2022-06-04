#pragma once

#include <memory>

#include "glm/glm.hpp"

#include "enviroment_system_interface.hpp"
#include "glew_headers.hpp"
#include "platform/glfw_window.hpp"


namespace blocks
{
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
  };
}
