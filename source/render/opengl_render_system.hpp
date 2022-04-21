#pragma once

#include <memory>

#include "glew_headers.hpp"

#include "opengl_shader.hpp"
#include "camera.hpp"

class OpenglRenderSystem 
{
public:
  OpenglRenderSystem();
  ~OpenglRenderSystem();

  void Init();
  void Deinit();

  void RenderScene(Camera* camera, float ratio);

private:
  unsigned int VBO;
  unsigned int VAO;
  std::unique_ptr<OpenglShader> defaultShader;
};
