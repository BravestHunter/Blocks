#pragma once

#include <memory>

#include "game_context.hpp"
#include "platform/glfw_window.hpp"
#include "shader_program.hpp"
#include "opengl_sprite.hpp"
#include "opengl_model.hpp"
#include "render/opengl_context.hpp"
#include "opengl_scene.hpp"
#include "simulation/camera.hpp"
#include "presentation/presentation_context.hpp"


namespace blocks
{
  class OpenglRenderModule
  {
  public:
    OpenglRenderModule();
    OpenglRenderModule(const OpenglRenderModule&) = delete;
    OpenglRenderModule(OpenglRenderModule&& other) = delete;
    OpenglRenderModule& operator=(const OpenglRenderModule&) = delete;
    OpenglRenderModule& operator=(OpenglRenderModule&& other) = delete;
    ~OpenglRenderModule();

    void Update(float delta, PresentationContext& presentationContext, GameContext& gameContext);

    void SetViewportSize(glm::ivec2 size);

    void SetContext(GlfwWindow& window);
    void InitResources(PresentationContext& presentationContext);
    void FreeResources(PresentationContext& presentationContext);

  private:
    std::unique_ptr<OpenglContext> context_;
    std::shared_ptr<opengl::ShaderProgram> chunkProgram_;
    std::shared_ptr<opengl::ShaderProgram> modelProgram_;
    std::shared_ptr<opengl::ShaderProgram> primitiveProgram_;
    std::shared_ptr<opengl::ShaderProgram> spriteProgram_;

    std::shared_ptr<OpenglModel> carModel_;
    std::shared_ptr<OpenglModel> aabbModel_;
    std::unique_ptr<OpenglSprite> crosshairSprite_;

    bool IsCorrectThread();
    void Clear(glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    void RenderChunks(std::shared_ptr<OpenglMap> map, glm::mat4 viewProjection);
    void RenderHUD();
    void RenderPhysicsBounds(std::unordered_map<entt::entity, AABB>& bounds, glm::mat4 viewProjection);

    static std::shared_ptr<OpenglModel> CreateAABBPresentationModel();
  };
}
