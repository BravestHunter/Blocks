#include "physics_module.hpp"

#include "geometry/collisions_api.hpp"
#include "simulation/player_position_changed_event.hpp"
#include "entity_physics_body_changed_event.hpp"


namespace blocks
{
  const float gravityConstant = 9.8f;
  const glm::vec3 zeroVector(0.0f);
  const glm::vec3 halfVector(0.5f);
  const glm::vec3 oneVector(1.0f);


  void PhysicsModule::Update(float delta, GameContext& gameContext)
  {
    std::shared_ptr<World> world = gameContext.scene->GetWorld();
    entt::registry& ecsRegistry = world->GetEcsRegistry();
    entt::entity playerEntity = world->GetPlayerEntity();
    auto view = ecsRegistry.view<Transform, PhysicsBody>();

    for (auto [entity, transform, physicsBody] : view.each())
    {
      glm::vec3 velocity = physicsBody.velocity;

      bool isPlayer = playerEntity == entity;

      if (isPlayer && gameContext.controlMode == ControlMode::Fly)
      {
        if (velocity == zeroVector)
        {
          return;
        }

        glm::vec3 newPosition = transform.position + velocity * delta;
        transform.position = newPosition;
       
        if (isPlayer)
        {
          gameContext.camera->SetPosition(newPosition);
          gameContext.modelUpdateEventsQueue.Push(std::make_shared<PlayerPositionChangedEvent>(transform.position));
        }
        else
        {
          gameContext.modelUpdateEventsQueue.Push(std::make_shared<EntityPhysicsBodyChangedEvent>(entity, physicsBody, transform));
        }
      }
      else
      {
        // Add gravity force
        velocity += glm::vec3(0.0f, 0.0f, -1.0f) * gravityConstant * delta;
        physicsBody.velocity = velocity;

        if (velocity == zeroVector)
        {
          return;
        }

        // Process horizontal and vertical movement separately
        glm::vec3 horizontalVelocity(velocity.x, velocity.y, 0.0f);
        glm::vec3 verticalVelocity(0.0f, 0.0f, velocity.z);

        bool horizontalMoved = ProcessVelocity(delta, transform, physicsBody, horizontalVelocity, gameContext);
        bool verticalMoved = ProcessVelocity(delta, transform, physicsBody, verticalVelocity, gameContext);

        physicsBody.isGrounded = !verticalMoved;
        physicsBody.horizontalCollision = !horizontalMoved && glm::length(horizontalVelocity) > 0.0f;

        if (horizontalMoved == false)
        {
          velocity.x = 0.0f;
          velocity.y = 0.0f;
        }
        if (verticalMoved == false)
        {
          velocity.z = 0.0f;
        }
        physicsBody.velocity = velocity;

        if (horizontalMoved || verticalMoved)
        {
          if (isPlayer)
          {
            gameContext.camera->SetPosition(transform.position);
            gameContext.modelUpdateEventsQueue.Push(std::make_shared<PlayerPositionChangedEvent>(transform.position));
          }
          else
          {
            gameContext.modelUpdateEventsQueue.Push(std::make_shared<EntityPhysicsBodyChangedEvent>(entity, physicsBody, transform));
          }
        }
      }
    }
  }


  bool PhysicsModule::ProcessVelocity(float delta, Transform& transform, PhysicsBody& physicsBody, glm::vec3 velocity, GameContext& gameContext)
  {
    glm::vec3 newPosition = transform.position + velocity * delta;

    {
      AABB playerBounds = physicsBody.bounds;
      AABB newPlayerBounds(playerBounds.center + newPosition, playerBounds.size);
      if (CollidesMap(newPlayerBounds, gameContext.scene->GetWorld()->GetMap()))
      {
        return false;
      }
    }

    transform.position = newPosition;

    return true;
  }

  bool PhysicsModule::CollidesMap(AABB& aabb, std::shared_ptr<Map> map)
  {
    ChunkPosition chunkPosition = Map::CalculateChunkPosition(aabb.center);
    std::shared_ptr<Chunk> chunk = map->GetChunk(chunkPosition);

    glm::vec3 localPosition = aabb.center - glm::vec3(chunkPosition.x * (int)Chunk::Length, chunkPosition.y * (int)Chunk::Width, 0.0f);
    AABB localBounds(localPosition, aabb.size);

    glm::ivec3 lowBorderBlock(localBounds.lowBorder);
    if (lowBorderBlock.x < 0) lowBorderBlock.x = 0;
    if (lowBorderBlock.y < 0) lowBorderBlock.y = 0;
    if (lowBorderBlock.z < 0) lowBorderBlock.z = 0;
    glm::ivec3 highBorderBlock(localBounds.highBorder);
    if (highBorderBlock.x >= Chunk::Length) highBorderBlock.x = Chunk::Length - 1;
    if (highBorderBlock.y >= Chunk::Width) highBorderBlock.y = Chunk::Width - 1;
    if (highBorderBlock.z >= Chunk::Height) highBorderBlock.z = Chunk::Height - 1;

    glm::uvec3 lowBorderBlockUnsigned = static_cast<glm::uvec3>(lowBorderBlock);
    glm::uvec3 highBorderBlockUnsigned = static_cast<glm::uvec3>(highBorderBlock);

    for (unsigned int x = lowBorderBlockUnsigned.x; x <= highBorderBlockUnsigned.x; x++)
    {
      for (unsigned int y = lowBorderBlockUnsigned.y; y <= highBorderBlockUnsigned.y; y++)
      {
        for (unsigned int z = lowBorderBlockUnsigned.z; z <= highBorderBlockUnsigned.z; z++)
        {
          size_t blockIndex = Chunk::CalculateBlockIndex(x, y, z);
          if (chunk->blocks[blockIndex] == 0)
          {
            continue;
          }

          AABB blockBounds(glm::vec3(x, y, z) + halfVector, oneVector);
          if (CheckCollision(blockBounds, localBounds))
          {
            return true;
          }
        }
      }
    }

    return false;
  }
}
