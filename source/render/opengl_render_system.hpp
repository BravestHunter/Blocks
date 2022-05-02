#pragma once

#include <memory>

#include "glew_headers.hpp"

#include "opengl_buffer.hpp"
#include "opengl_vertex_array_object.hpp"
#include "opengl_model.hpp"
#include "opengl_chunk.hpp"
#include "opengl_map.hpp"
#include "opengl_texture.hpp"
#include "opengl_shader.hpp"
#include "camera.hpp"
#include "model/chunk.hpp"


class OpenglRenderSystem 
{
public:
  OpenglRenderSystem();
  ~OpenglRenderSystem();

  void Init();
  void Deinit();

  void Clear(glm::vec4 clearColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  void RenderModel(std::shared_ptr<OpenglModel> model, glm::mat4 modelTransform, Camera* camera, float ratio);
  void RenderChunk(std::shared_ptr<OpenglChunk> chunk, Camera* camera, float ratio);
  void RenderMap(std::shared_ptr<OpenglMap> map, Camera* camera, float ratio);

  void SetWireframeMode(bool value);

private:
  std::unique_ptr<OpenglShader> defaultShader_;
};
