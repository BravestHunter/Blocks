#include "player_control_module.hpp"

#include <iostream>

#include "simulation/player_position_changed_event.hpp"
#include "simulation/chunk_updated_event.hpp"


namespace blocks
{
  glm::vec3 zeroVector(0.0f, 0.0f, 0.0f);

  PlayerControlModule::PlayerControlModule()
  {

  }

  PlayerControlModule::~PlayerControlModule()
  {

  }

  void PlayerControlModule::Update(const float delta, const InputState& inputState, GameContext& gameContext)
  {
    MovePlayer(delta, inputState, gameContext);
    RotateCamera(delta, inputState, gameContext);
    ManageBlockPlacement(delta, inputState, gameContext);
  }


  void PlayerControlModule::MovePlayer(const float delta, const InputState& inputState, GameContext& gameContext)
  {
    Entity& player = gameContext.scene->GetWorld()->GetPlayer();

    if (gameContext.controlMode == ControlMode::Default)
    {
      if (player.IsGrounded() && inputState.IsKeyPressed(GLFW_KEY_SPACE))
      {
        player.SetVelocity(player.GetVelocity() + glm::vec3(0.0, 0.0, 500.0f) * delta);
      }

      player.SetVelocity(glm::vec3(0.0f, 0.0f, player.GetVelocity().z));

      glm::vec3 direction = glm::vec3(0.0f);
      if (inputState.IsKeyPressed(GLFW_KEY_W))
      {
        direction += gameContext.camera->GetForward();
      }
      if (inputState.IsKeyPressed(GLFW_KEY_S))
      {
        direction -= gameContext.camera->GetForward();
      }
      if (inputState.IsKeyPressed(GLFW_KEY_A))
      {
        direction -= gameContext.camera->GetRight();
      }
      if (inputState.IsKeyPressed(GLFW_KEY_D))
      {
        direction += gameContext.camera->GetRight();
      }

      if (direction.x != 0 || direction.y != 0)
      {
        direction.z = 0.0f;
        direction = glm::normalize(direction);
        player.SetVelocity(player.GetVelocity() + direction * movementSpeed_);
      }
    }
    else if (gameContext.controlMode == ControlMode::Fly)
    {
      player.SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));

      glm::vec3 direction = glm::vec3(0.0f);
      if (inputState.IsKeyPressed(GLFW_KEY_W))
      {
        direction += gameContext.camera->GetForward();
      }
      if (inputState.IsKeyPressed(GLFW_KEY_S))
      {
        direction -= gameContext.camera->GetForward();
      }
      if (inputState.IsKeyPressed(GLFW_KEY_A))
      {
        direction -= gameContext.camera->GetRight();
      }
      if (inputState.IsKeyPressed(GLFW_KEY_D))
      {
        direction += gameContext.camera->GetRight();
      }

      if (direction != zeroVector)
      {
        direction = glm::normalize(direction);
        player.SetVelocity(direction * flyMovementSpeed_);
      }
    }
  }

  void PlayerControlModule::RotateCamera(const float delta, const InputState& inputState, GameContext& gameContext)
  {
    const glm::vec2 offset = inputState.GetCursorOffset();
    
    glm::vec2 actualOffset = offset * mouseSensitivity_;
    
    gameContext.camera->SetYaw(gameContext.camera->GetYaw() + actualOffset.x);
    gameContext.camera->SetPitch(gameContext.camera->GetPitch() + actualOffset.y);
  }

  void PlayerControlModule::ManageBlockPlacement(const float delta, const InputState& inputState, GameContext& gameContext)
  {
    if (inputState.IsMouseButtonJustPressed(GLFW_MOUSE_BUTTON_1))
    {
      BlockLookAt blockLookAt = gameContext.scene->GetWorld()->GetMap()->GetBlockLookAt(blocks::Ray(gameContext.camera->GetPosition(), gameContext.camera->GetForward()));
      if (!blockLookAt.hit)
      {
        return;
      }

      ChunkPosition placeChunkPosition = blockLookAt.chunkPosition;
      glm::ivec3 placeBlockPosition = blockLookAt.blockPosition;
      switch (blockLookAt.loockFromDirection)
      {
      case Direction::Forward:
        if (blockLookAt.blockPosition.x == Chunk::Length - 1)
        {
          placeChunkPosition.first += 1;
          placeBlockPosition.x = 0;
        }
        else
        {
          placeBlockPosition.x++;
        }
        break;
      case Direction::Back:
        if (blockLookAt.blockPosition.x == 0)
        {
          placeChunkPosition.first -= 1;
          placeBlockPosition.x = 15;
        }
        else
        {
          placeBlockPosition.x--;
        }
        break;

      case Direction::Right:
        if (blockLookAt.blockPosition.y == Chunk::Width - 1)
        {
          placeChunkPosition.second += 1;
          placeBlockPosition.y = 0;
        }
        else
        {
          placeBlockPosition.y++;
        }
        break;
      case Direction::Left:
        if (blockLookAt.blockPosition.y == 0)
        {
          placeChunkPosition.second -= 1;
          placeBlockPosition.y = 15;
        }
        else
        {
          placeBlockPosition.y--;
        }
        break;

      case Direction::Up:
        if (blockLookAt.blockPosition.z == Chunk::Height - 1)
        {
          return;
        }
        else
        {
          placeBlockPosition.z++;
        }
        break;
      case Direction::Down:
        if (blockLookAt.blockPosition.z == 0)
        {
          return;
        }
        else
        {
          placeBlockPosition.z--;
        }
        break;
      }

      std::shared_ptr<Chunk> chunk = gameContext.scene->GetWorld()->GetMap()->GetChunk(placeChunkPosition);
      chunk->blocks[placeBlockPosition.x + placeBlockPosition.y * Chunk::Width + placeBlockPosition.z * Chunk::LayerBlocksNumber] = 1;
      gameContext.modelUpdateEventsQueue.Push(std::make_shared<ChunkUpdatedEvent>(placeChunkPosition));
    }
    else if (inputState.IsMouseButtonJustPressed(GLFW_MOUSE_BUTTON_2))
    {
      BlockLookAt blockLookAt = gameContext.scene->GetWorld()->GetMap()->GetBlockLookAt(blocks::Ray(gameContext.camera->GetPosition(), gameContext.camera->GetForward()));
      if (!blockLookAt.hit)
      {
        return;
      }

      std::shared_ptr<Chunk> chunk = gameContext.scene->GetWorld()->GetMap()->GetChunk(blockLookAt.chunkPosition);
      chunk->blocks[blockLookAt.blockPosition.x + blockLookAt.blockPosition.y * Chunk::Width + blockLookAt.blockPosition.z * Chunk::LayerBlocksNumber] = 0;
      gameContext.modelUpdateEventsQueue.Push(std::make_shared<ChunkUpdatedEvent>(blockLookAt.chunkPosition));
    }
  }
}
