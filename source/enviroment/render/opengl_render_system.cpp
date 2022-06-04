#include "opengl_render_system.hpp"

#include <exception>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "compile_utils.hpp"
#include "resourceConfig.h"
#include "io/file_api.hpp"


namespace blocks
{
  OpenglRenderSystem::OpenglRenderSystem()
  {

  }

  OpenglRenderSystem::~OpenglRenderSystem()
  {

  }


  void OpenglRenderSystem::Init()
  {
    // Nothing to do here
  }

  void OpenglRenderSystem::Deinit()
  {
    // Nothing to do here
  }

  bool OpenglRenderSystem::IsInitialized()
  {
    return true;
  }


  void OpenglRenderSystem::OnContextChanged()
  {
    GLenum initResult = glewInit();
    if (initResult != GLEW_OK)
    {
      throw std::exception("Failed to initialize GLEW");
    }
  }


  void OpenglRenderSystem::Clear(glm::vec4 clearColor)
  {
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }
}
