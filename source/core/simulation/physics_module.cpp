#include "physics_module.hpp"

#include "simulation/player_position_changed_event.hpp"


namespace blocks
{
  const float gravityConstant = 9.8f;

  void PhysicsModule::Update(float delta, GameContext& gameContext)
  {
    Entity& player = gameContext.scene->GetWorld()->GetPlayer();

    glm::vec3 velocity = player.GetVelocity();

    if (gameContext.isPhysicsEnabled)
    {
      velocity += glm::vec3(0.0f, 0.0f, -1.0f) * gravityConstant * delta;
      player.SetVelocity(velocity);
    }

    if (velocity.x != 0 || velocity.y != 0 || velocity.z != 0)
    {
      glm::vec3 position = player.GetPosition();
      position += velocity * delta;
      player.SetPosition(position);

      gameContext.camera->SetPosition(position);

      gameContext.modelUpdateEventsQueue.Push(std::make_shared<PlayerPositionChangedEvent>(position));
    }
  }
}
