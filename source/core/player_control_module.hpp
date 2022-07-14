#pragma once

#include "game_module_interface.hpp"
#include "platform/glfw_window.hpp"
#include "input/input_state.hpp"
#include "render/opengl_render_module.hpp"


namespace blocks
{
  class PlayerControlModule
  {
  public:
    PlayerControlModule();
    PlayerControlModule(const PlayerControlModule&) = delete;
    PlayerControlModule(PlayerControlModule&& other) = delete;
    PlayerControlModule& operator=(const PlayerControlModule&) = delete;
    PlayerControlModule& operator=(PlayerControlModule&& other) = delete;
    ~PlayerControlModule();

    virtual void Update(const float delta, const InputState& inputState, GameContext& context);

  private:
    void MovePlayer(const float delta, const InputState& inputState, GameContext& context);
    void RotateCamera(const float delta, const InputState& inputState, GameContext& context);
    void ZoomCamera(const float delta, const InputState& inputState, GameContext& context);
    void ManageBlockPlacement(const float delta, const InputState& inputState, GameContext& context);
  };
}