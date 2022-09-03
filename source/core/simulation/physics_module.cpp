#include "physics_module.hpp"

#include "geometry/collisions_api.hpp"

#include "simulation/player_position_changed_event.hpp"


namespace blocks
{
  const float gravityConstant = 9.8f;
  const glm::vec3 zeroVector(0.0f);
  const glm::vec3 halfVector(0.5f);
  const glm::vec3 oneVector(1.0f);


  void PhysicsModule::Update(float delta, GameContext& gameContext)
  {
    Entity& player = gameContext.scene->GetWorld()->GetPlayer();

    glm::vec3 velocity = player.GetVelocity();

    if (gameContext.isGravityEnabled)
    {
      velocity += glm::vec3(0.0f, 0.0f, -1.0f) * gravityConstant * delta;
      player.SetVelocity(velocity);
    }

    if (velocity == zeroVector)
    {
      return;
    }

    // Process horizontal and vertical movement separately
    glm::vec3 horizontalVelocity(velocity.x, velocity.y, 0.0f);
    glm::vec3 verticalVelocity(0.0f, 0.0f, velocity.z);

    bool horizontalMoved = ProcessPlayerVelocity(delta, player, horizontalVelocity, gameContext);
    bool verticalMoved = ProcessPlayerVelocity(delta, player, verticalVelocity, gameContext);

    if (horizontalMoved == false)
    {
      velocity.x = 0.0f;
      velocity.y = 0.0f;
    }
    if (verticalMoved == false)
    {
      velocity.z = 0.0f;
    }
    player.SetVelocity(velocity);

    if (horizontalMoved || verticalMoved)
    {
      gameContext.modelUpdateEventsQueue.Push(std::make_shared<PlayerPositionChangedEvent>(player.GetPosition()));
    }
  }


  bool PhysicsModule::ProcessPlayerVelocity(float delta, Entity& player, glm::vec3 velocity, GameContext& gameContext)
  {
    glm::vec3 newPosition = player.GetPosition() + velocity * delta;

    {
      AABB playerBounds = player.GetAABB();
      AABB newPlayerBounds(playerBounds.center + newPosition, playerBounds.size);
      if (CollidesMap(newPlayerBounds, gameContext.scene->GetWorld()->GetMap()))
      {
        return false;
      }
    }

    player.SetPosition(newPosition);
    gameContext.camera->SetPosition(newPosition);

    return true;
  }

  bool PhysicsModule::CollidesMap(AABB& aabb, std::shared_ptr<Map> map)
  {
    ChunkPosition chunkPosition = Map::CalculateChunkPosition(aabb.center);
    std::shared_ptr<Chunk> chunk = map->GetChunk(chunkPosition);

    glm::vec3 localPosition = aabb.center - glm::vec3(chunkPosition.first * (int)Chunk::Length, chunkPosition.second * (int)Chunk::Width, 0.0f);
    AABB localBounds(localPosition, aabb.size);

    glm::ivec3 lowBorderBlock(localBounds.lowBorder);
    if (lowBorderBlock.x < 0) lowBorderBlock.x = 0;
    if (lowBorderBlock.y < 0) lowBorderBlock.y = 0;
    if (lowBorderBlock.z < 0) lowBorderBlock.z = 0;
    glm::ivec3 highBorderBlock(localBounds.highBorder);
    if (highBorderBlock.x >= Chunk::Length) highBorderBlock.x = Chunk::Length - 1;
    if (highBorderBlock.y >= Chunk::Width) highBorderBlock.y = Chunk::Width - 1;
    if (highBorderBlock.z >= Chunk::Height) highBorderBlock.z = Chunk::Height - 1;

    for (int x = lowBorderBlock.x; x <= highBorderBlock.x; x++)
    {
      for (int y = lowBorderBlock.y; y <= highBorderBlock.y; y++)
      {
        for (int z = lowBorderBlock.z; z <= highBorderBlock.z; z++)
        {
          if (chunk->blocks[x + y * Chunk::Length + z * Chunk::LayerBlocksNumber] == 0)
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
