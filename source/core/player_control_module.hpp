#pragma once

#include "game_module_interface.hpp"
#include "platform/glfw_window.hpp"
#include "input/input_state.hpp"


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
    float movementSpeed_ = 32.0f;
    float mouseSensitivity_ = 0.1f;

    void MovePlayer(const float delta, const InputState& inputState, GameContext& context);
    void RotateCamera(const float delta, const InputState& inputState, GameContext& context);
    void ManageBlockPlacement(const float delta, const InputState& inputState, GameContext& context);
  };
}