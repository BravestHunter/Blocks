#include "player_control_module.hpp"


PlayerControlModule::PlayerControlModule()
{

}

PlayerControlModule::~PlayerControlModule()
{

}

void PlayerControlModule::Update(float delta, GameContext& context)
{
  if (context.scene->ContainsMap())
  {
    MovePlayer(delta, context);
  }
}


void PlayerControlModule::SetCallbacks(GlfwWindow& window, GameContext& context, OpenglRenderModule& renderModule)
{
  window.SetCursorPositionCallback(
    [&context](double xpos, double ypos)
    {
      float xposF = static_cast<float>(xpos);
      float yposF = static_cast<float>(ypos);

      if (context.firstMouse)
      {
        context.lastMouseX = xposF;
        context.lastMouseY = yposF;
        context.firstMouse = false;
      }

      float xoffset = xposF - context.lastMouseX;
      float yoffset = context.lastMouseY - yposF; // reversed since y-coordinates go from bottom to top

      context.lastMouseX = xposF;
      context.lastMouseY = yposF;

      if (!context.isCursorEnabled)
      {
        context.camera->ProcessMouseMovement(xoffset, yoffset);
      }
    }
  );
  window.SetScrollCallback(
    [&context](double xoffset, double yoffset)
    {
      context.camera->ProcessMouseScroll(static_cast<float>(yoffset));
    }
  );
  window.SetMouseButtonCallback(
    [&context, &renderModule](int button, int action, int mods)
    {
      if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) // Place block
      {
        if (context.scene->ContainsMap() && !context.isCursorEnabled)
        {
          BlockLookAt blockLookAt = context.scene->GetMap()->GetBlockLookAt(Ray(context.camera->GetPosition(), context.camera->GetForward()));
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
          renderModule.GetOpenglScene()->AddChunk(chunk, placeChunkPosition);
        }
      }
      else if (button == GLFW_MOUSE_BUTTON_2 && action == GLFW_PRESS) // Remove block
      {
        if (context.scene->ContainsMap() && !context.isCursorEnabled)
        {
          BlockLookAt blockLookAt = context.scene->GetMap()->GetBlockLookAt(Ray(context.camera->GetPosition(), context.camera->GetForward()));
          if (!blockLookAt.hit)
          {
            return;
          }

          std::shared_ptr<Chunk> chunk = context.scene->GetMap()->GetChunk(blockLookAt.chunkPosition);
          chunk->blocks[blockLookAt.blockPosition.x + blockLookAt.blockPosition.y * Chunk::Width + blockLookAt.blockPosition.z * Chunk::LayerBlocksNumber] = 0;
          renderModule.GetOpenglScene()->AddChunk(chunk, blockLookAt.chunkPosition);
        }
      }
    }
  );
}



void PlayerControlModule::MovePlayer(float delta, GameContext& context)
{
  float fixedDelta = 0.016f;
  float velocity = 32.0f * fixedDelta;

  glm::vec3 position = context.camera->GetPosition();
  glm::vec3 shift = glm::vec3(0.0f);

  if (context.isWPressed)
  {
    shift += context.camera->GetForward() * velocity;
  }
  if (context.isSPressed)
  {
    shift -= context.camera->GetForward() * velocity;
  }
  if (context.isAPressed)
  {
    shift -= context.camera->GetRight() * velocity;
  }
  if (context.isDPressed)
  {
    shift += context.camera->GetRight() * velocity;
  }

  position += shift;
  if (!context.scene->GetMap()->Collides(context.playerBounds, position))
  {
    context.camera->SetPosition(position);
  }
}

