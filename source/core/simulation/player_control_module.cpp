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
    bool chunkChanged = false;
    ChunkPosition changedChunkPosition;
    glm::ivec3 changedBlockPosition;

    if (inputState.IsMouseButtonJustPressed(GLFW_MOUSE_BUTTON_2))
    {
      Ray selectionRay(gameContext.camera->GetPosition(), gameContext.camera->GetForward());
      BlockLookAt blockLookAt = gameContext.scene->GetWorld()->GetMap()->GetBlockLookAt(selectionRay);

      if (!blockLookAt.hit)
      {
        return;
      }

      chunkChanged = true;
      changedChunkPosition = blockLookAt.chunkPosition;
      changedBlockPosition = blockLookAt.blockPosition;

      switch (blockLookAt.intersectionSide)
      {
      case BlockSide::Front:
        if (blockLookAt.blockPosition.x == Chunk::Length - 1)
        {
          changedChunkPosition.first += 1;
          changedBlockPosition.x = 0;
        }
        else
        {
          changedBlockPosition.x++;
        }
        break;
      case BlockSide::Back:
        if (blockLookAt.blockPosition.x == 0)
        {
          changedChunkPosition.first -= 1;
          changedBlockPosition.x = 15;
        }
        else
        {
          changedBlockPosition.x--;
        }
        break;

      case BlockSide::Right:
        if (blockLookAt.blockPosition.y == Chunk::Width - 1)
        {
          changedChunkPosition.second += 1;
          changedBlockPosition.y = 0;
        }
        else
        {
          changedBlockPosition.y++;
        }
        break;
      case BlockSide::Left:
        if (blockLookAt.blockPosition.y == 0)
        {
          changedChunkPosition.second -= 1;
          changedBlockPosition.y = 15;
        }
        else
        {
          changedBlockPosition.y--;
        }
        break;

      case BlockSide::Top:
        if (blockLookAt.blockPosition.z == Chunk::Height - 1)
        {
          return;
        }
        else
        {
          changedBlockPosition.z++;
        }
        break;
      case BlockSide::Bottom:
        if (blockLookAt.blockPosition.z == 0)
        {
          return;
        }
        else
        {
          changedBlockPosition.z--;
        }
        break;
      }

      std::shared_ptr<Chunk> chunk = gameContext.scene->GetWorld()->GetMap()->GetChunk(changedChunkPosition);
      size_t blockIndex = Chunk::CalculateBlockIndex(changedBlockPosition);
      chunk->blocks[blockIndex] = 1;
    }
    else if (inputState.IsMouseButtonJustPressed(GLFW_MOUSE_BUTTON_1))
    {
      Ray selectionRay(gameContext.camera->GetPosition(), gameContext.camera->GetForward());
      BlockLookAt blockLookAt = gameContext.scene->GetWorld()->GetMap()->GetBlockLookAt(selectionRay);

      if (!blockLookAt.hit)
      {
        return;
      }

      chunkChanged = true;
      changedChunkPosition = blockLookAt.chunkPosition;
      changedBlockPosition = blockLookAt.blockPosition;

      std::shared_ptr<Chunk> chunk = gameContext.scene->GetWorld()->GetMap()->GetChunk(changedChunkPosition);
      size_t blockIndex = Chunk::CalculateBlockIndex(changedBlockPosition);
      chunk->blocks[blockIndex] = 0;
    }

    if (chunkChanged)
    {
      gameContext.modelUpdateEventsQueue.Push(std::make_shared<ChunkUpdatedEvent>(changedChunkPosition));

      if (changedBlockPosition.x == 0)
      {
        gameContext.modelUpdateEventsQueue.Push(std::make_shared<ChunkUpdatedEvent>(ChunkPosition(changedChunkPosition.first - 1, changedChunkPosition.second)));
      }
      else if (changedBlockPosition.x == Chunk::Length - 1)
      {
        gameContext.modelUpdateEventsQueue.Push(std::make_shared<ChunkUpdatedEvent>(ChunkPosition(changedChunkPosition.first + 1, changedChunkPosition.second)));
      }
      if (changedBlockPosition.y == 0)
      {
        gameContext.modelUpdateEventsQueue.Push(std::make_shared<ChunkUpdatedEvent>(ChunkPosition(changedChunkPosition.first, changedChunkPosition.second - 1)));
      }
      else if (changedBlockPosition.y == Chunk::Width - 1)
      {
        gameContext.modelUpdateEventsQueue.Push(std::make_shared<ChunkUpdatedEvent>(ChunkPosition(changedChunkPosition.first, changedChunkPosition.second + 1)));
      }
    }
  }
}
