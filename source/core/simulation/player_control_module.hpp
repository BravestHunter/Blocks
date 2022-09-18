#pragma once

#include "platform/glfw_window.hpp"
#include "game_context.hpp"
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

    virtual void Update(const float delta, const InputState& inputState, GameContext& gameContext);

  private:
    float movementSpeed_ = 8.0f;
    float flyMovementSpeed_ = 20.0f;
    float mouseSensitivity_ = 0.1f;

    void MovePlayer(const float delta, const InputState& inputState, GameContext& gameContext);
    void RotateCamera(const float delta, const InputState& inputState, GameContext& gameContext);
    void ManageBlockPlacement(const float delta, const InputState& inputState, GameContext& gameContext);
  };
}