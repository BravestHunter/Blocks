#include "player_control_module.hpp"

#include <iostream>

namespace blocks
{
  PlayerControlModule::PlayerControlModule()
  {

  }

  PlayerControlModule::~PlayerControlModule()
  {

  }

  void PlayerControlModule::Update(const float delta, const InputState& inputState, GameContext& context)
  {
    if (context.scene->ContainsMap())
    {
      MovePlayer(delta, inputState, context);
      RotateCamera(delta, inputState, context);
      ZoomCamera(delta, inputState, context);
      ManageBlockPlacement(delta, inputState, context);
    }
  }


  void PlayerControlModule::MovePlayer(const float delta, const InputState& inputState, GameContext& context)
  {
    const float velocity = delta * 32.0f;

    glm::vec3 position = context.camera->GetPosition();
    glm::vec3 shift = glm::vec3(0.0f);

    if (inputState.IsKeyPressed(GLFW_KEY_W))
    {
      shift += context.camera->GetForward() * velocity;
    }
    if (inputState.IsKeyPressed(GLFW_KEY_S))
    {
      shift -= context.camera->GetForward() * velocity;
    }
    if (inputState.IsKeyPressed(GLFW_KEY_A))
    {
      shift -= context.camera->GetRight() * velocity;
    }
    if (inputState.IsKeyPressed(GLFW_KEY_D))
    {
      shift += context.camera->GetRight() * velocity;
    }

    position += shift;
    context.camera->SetPosition(position);
    //if (!context.scene->GetMap()->Collides(context.playerBounds, position))
    //{
    //  context.camera->SetPosition(position);
    //}
  }

  void PlayerControlModule::RotateCamera(const float delta, const InputState& inputState, GameContext& context)
  {
    if (!context.isCursorEnabled)
    {
      const glm::vec2 offset = inputState.GetCursorOffset();
      context.camera->ProcessMouseMovement(offset.x, -offset.y);
    }
  }

  void PlayerControlModule::ZoomCamera(const float delta, const InputState& inputState, GameContext& context)
  {
    if (!context.isCursorEnabled)
    {
      const glm::vec2 offset = inputState.GetScrollOffset();
      context.camera->ProcessMouseScroll(offset.y);
    }
  }

  void PlayerControlModule::ManageBlockPlacement(const float delta, const InputState& inputState, GameContext& context)
  {
    if (inputState.IsMouseButtonJustPressed(GLFW_MOUSE_BUTTON_1))
    {
      if (context.scene->ContainsMap() && !context.isCursorEnabled)
      {
        BlockLookAt blockLookAt = context.scene->GetMap()->GetBlockLookAt(blocks::Ray(context.camera->GetPosition(), context.camera->GetForward()));
        if (!blockLookAt.hit)
        {
          return;
        }

        std::pair<int, int> placeChunkPosition = blockLookAt.chunkPosition;
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

        std::shared_ptr<Chunk> chunk = context.scene->GetMap()->GetChunk(placeChunkPosition);
        chunk->blocks[placeBlockPosition.x + placeBlockPosition.y * Chunk::Width + placeBlockPosition.z * Chunk::LayerBlocksNumber] = 1;
        context.openglScene->GetMap()->EnqueueChunkAdd(context.scene->GetMap(), placeChunkPosition);
      }
    }
    else if (inputState.IsMouseButtonJustPressed(GLFW_MOUSE_BUTTON_2))
    {
      if (context.scene->ContainsMap() && !context.isCursorEnabled)
      {
        BlockLookAt blockLookAt = context.scene->GetMap()->GetBlockLookAt(blocks::Ray(context.camera->GetPosition(), context.camera->GetForward()));
        if (!blockLookAt.hit)
        {
          return;
        }

        std::shared_ptr<Chunk> chunk = context.scene->GetMap()->GetChunk(blockLookAt.chunkPosition);
        chunk->blocks[blockLookAt.blockPosition.x + blockLookAt.blockPosition.y * Chunk::Width + blockLookAt.blockPosition.z * Chunk::LayerBlocksNumber] = 0;
        context.openglScene->GetMap()->EnqueueChunkAdd(context.scene->GetMap(), blockLookAt.chunkPosition);
      }
    }
  }
}
