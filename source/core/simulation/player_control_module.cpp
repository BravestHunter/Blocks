#include "player_control_module.hpp"

#include "ecs/components/transform.hpp"
#include "ecs/components/physics_body.hpp"
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
    std::shared_ptr<World> world = gameContext.scene->GetWorld();
    entt::registry& ecsRegistry = world->GetEcsRegistry();
    entt::entity playerEntity = world->GetPlayerEntity();
    PhysicsBody& physicsBody = ecsRegistry.get<PhysicsBody>(playerEntity);

    if (gameContext.controlMode == ControlMode::Default)
    {
      if (physicsBody.isGrounded && inputState.IsKeyPressed(GLFW_KEY_SPACE))
      {
        physicsBody.velocity = physicsBody.velocity + glm::vec3(0.0, 0.0, 500.0f) * delta;
      }

      physicsBody.velocity = glm::vec3(0.0f, 0.0f, physicsBody.velocity.z);

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
        physicsBody.velocity = physicsBody.velocity + direction * movementSpeed_;
      }
    }
    else if (gameContext.controlMode == ControlMode::Fly)
    {
      physicsBody.velocity = glm::vec3(0.0f, 0.0f, 0.0f);

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
        physicsBody.velocity = direction * flyMovementSpeed_;
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
    std::shared_ptr<World> world = gameContext.scene->GetWorld();
    entt::registry& ecsRegistry = world->GetEcsRegistry();
    entt::entity playerEntity = world->GetPlayerEntity();
    Transform& transform = ecsRegistry.get<Transform>(playerEntity);
    PhysicsBody& physicsBody = ecsRegistry.get<PhysicsBody>(playerEntity);

    bool chunkChanged = false;
    ChunkPosition changedChunkPosition;
    glm::ivec3 changedBlockPosition;

    if (inputState.IsMouseButtonJustPressed(GLFW_MOUSE_BUTTON_2))
    {
      Ray selectionRay(gameContext.camera->GetPosition(), gameContext.camera->GetForward());
      MapRayCastResult raycastResult = gameContext.scene->GetWorld()->GetMap()->RayCast(selectionRay, 16.0f);

      if (!raycastResult.hit || raycastResult.intersectedSide == BlockSide::Unknown)
      {
        return;
      }

      chunkChanged = true;
      changedChunkPosition = raycastResult.chunkPosition;
      changedBlockPosition = raycastResult.blockPosition;

      switch (raycastResult.intersectedSide)
      {
      case BlockSide::Front:
        if (raycastResult.blockPosition.x == Chunk::Length - 1)
        {
          changedChunkPosition.x += 1;
          changedBlockPosition.x = 0;
        }
        else
        {
          changedBlockPosition.x++;
        }
        break;
      case BlockSide::Back:
        if (raycastResult.blockPosition.x == 0)
        {
          changedChunkPosition.x -= 1;
          changedBlockPosition.x = 15;
        }
        else
        {
          changedBlockPosition.x--;
        }
        break;

      case BlockSide::Right:
        if (raycastResult.blockPosition.y == Chunk::Width - 1)
        {
          changedChunkPosition.y += 1;
          changedBlockPosition.y = 0;
        }
        else
        {
          changedBlockPosition.y++;
        }
        break;
      case BlockSide::Left:
        if (raycastResult.blockPosition.y == 0)
        {
          changedChunkPosition.y -= 1;
          changedBlockPosition.y = 15;
        }
        else
        {
          changedBlockPosition.y--;
        }
        break;

      case BlockSide::Top:
        if (raycastResult.blockPosition.z == Chunk::Height - 1)
        {
          return;
        }
        else
        {
          changedBlockPosition.z++;
        }
        break;
      case BlockSide::Bottom:
        if (raycastResult.blockPosition.z == 0)
        {
          return;
        }
        else
        {
          changedBlockPosition.z--;
        }
        break;
      }

      glm::vec3 localPlayerPosition
      (
        transform.position.x - raycastResult.chunkPosition.x * static_cast<float>(Chunk::Length), 
        transform.position.y - raycastResult.chunkPosition.y * static_cast<float>(Chunk::Width),
        transform.position.z
      );
      AABB playerBounds = AABB(physicsBody.bounds.center + localPlayerPosition, physicsBody.bounds.size);
      AABB blockBounds(glm::vec3(changedBlockPosition) + glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f));
      if (CheckCollision(playerBounds, blockBounds))
      {
        return;
      }

      std::shared_ptr<Chunk> chunk = gameContext.scene->GetWorld()->GetMap()->GetChunk(changedChunkPosition);
      size_t blockIndex = Chunk::CalculateBlockIndex(changedBlockPosition);
      if (chunk->blocks[blockIndex] == 0)
      {
        chunk->blocks[blockIndex] = 1;
      }
    }
    else if (inputState.IsMouseButtonJustPressed(GLFW_MOUSE_BUTTON_1))
    {
      Ray selectionRay(gameContext.camera->GetPosition(), gameContext.camera->GetForward());
      MapRayCastResult raycastResult = gameContext.scene->GetWorld()->GetMap()->RayCast(selectionRay, 16.0f);

      if (!raycastResult.hit)
      {
        return;
      }

      chunkChanged = true;
      changedChunkPosition = raycastResult.chunkPosition;
      changedBlockPosition = raycastResult.blockPosition;

      std::shared_ptr<Chunk> chunk = gameContext.scene->GetWorld()->GetMap()->GetChunk(changedChunkPosition);
      size_t blockIndex = Chunk::CalculateBlockIndex(changedBlockPosition);
      chunk->blocks[blockIndex] = 0;
    }

    if (chunkChanged)
    {
      gameContext.modelUpdateEventsQueue.Push(std::make_shared<ChunkUpdatedEvent>(changedChunkPosition));

      if (changedBlockPosition.x == 0)
      {
        gameContext.modelUpdateEventsQueue.Push(std::make_shared<ChunkUpdatedEvent>(ChunkPosition(changedChunkPosition.x - 1, changedChunkPosition.y)));
      }
      else if (changedBlockPosition.x == Chunk::Length - 1)
      {
        gameContext.modelUpdateEventsQueue.Push(std::make_shared<ChunkUpdatedEvent>(ChunkPosition(changedChunkPosition.x + 1, changedChunkPosition.y)));
      }
      if (changedBlockPosition.y == 0)
      {
        gameContext.modelUpdateEventsQueue.Push(std::make_shared<ChunkUpdatedEvent>(ChunkPosition(changedChunkPosition.x, changedChunkPosition.y - 1)));
      }
      else if (changedBlockPosition.y == Chunk::Width - 1)
      {
        gameContext.modelUpdateEventsQueue.Push(std::make_shared<ChunkUpdatedEvent>(ChunkPosition(changedChunkPosition.x, changedChunkPosition.y + 1)));
      }
    }
  }
}
