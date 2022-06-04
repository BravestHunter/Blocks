#pragma once

#include "game_module_interface.hpp"
#include "platform/glfw_window.hpp"
#include "render/opengl_render_module.hpp"


class PlayerControlModule : public GameModuleInterface
{
public:
  PlayerControlModule();
  PlayerControlModule(const PlayerControlModule&) = delete;
  PlayerControlModule(PlayerControlModule&& other) = delete;
  PlayerControlModule& operator=(const PlayerControlModule&) = delete;
  PlayerControlModule& operator=(PlayerControlModule&& other) = delete;
  ~PlayerControlModule() override;

  virtual void Update(float delta, GameContext& context) override;

  void SetCallbacks(GlfwWindow& window, GameContext& context, OpenglRenderModule& renderModule);

private:
  void MovePlayer(float delta, GameContext& context);
};
