#include "entity_spawn_module.hpp"

#include "ecs/components/transform.hpp"
#include "ecs/components/physics_body.hpp"
#include "entity_physics_body_changed_event.hpp"


namespace blocks
{
  EntitySpawnModule::EntitySpawnModule()
  {
  }

  EntitySpawnModule::~EntitySpawnModule()
  {
  }


  void EntitySpawnModule::Update(const float delta, const InputState& inputState, GameContext& gameContext)
  {
    if (inputState.IsKeyJustPressed(GLFW_KEY_B))
    {
      std::shared_ptr<World> world = gameContext.scene->GetWorld();
      entt::registry& ecsRegistry = world->GetEcsRegistry();
      const Transform& playerPosition = ecsRegistry.get<Transform>(world->GetPlayerEntity());

      entt::entity entity = ecsRegistry.create();

      Transform transform
      {
        .position = playerPosition.position
      };
      ecsRegistry.emplace<Transform>(entity, transform);

      PhysicsBody physicsBody
      {
        .bounds = AABB(glm::vec3(0.0f), glm::vec3(0.5f, 0.5f, 1.0f))
      };
      ecsRegistry.emplace<PhysicsBody>(entity, physicsBody);

      gameContext.modelUpdateEventsQueue.Push(std::make_shared<EntityPhysicsBodyChangedEvent>(entity, physicsBody, transform));
    }
  }
}
