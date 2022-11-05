#include "entity_ai_module.hpp"

#include "ecs/components/transform.hpp"
#include "ecs/components/physics_body.hpp"
#include "ecs/components/ai_component.hpp"


namespace blocks
{
  const float idleTime = 4.0f;
  const float wanderTime = 4.0f;
  const float jumpVelocity = 5.0f;

  EntityAiModule::EntityAiModule()
  {
  }


  void EntityAiModule::Update(const float delta, const TimeState& timeState, const InputState& inputState, GameContext& gameContext)
  {
    std::shared_ptr<World> world = gameContext.scene->GetWorld();
    entt::registry& ecsRegistry = world->GetEcsRegistry();
    entt::entity playerEntity = world->GetPlayerEntity();
    auto view = ecsRegistry.view<Transform, PhysicsBody, AiComponent>();

    for (auto [entity, transform, physicsBody, aiComponent] : view.each())
    {
      float currentTime = static_cast<float>(timeState.GetSceneTime());
      float timeSinceBehaviorChanged = currentTime - aiComponent.behaviorChangedTime;

      // Try to switch behavior
      switch (aiComponent.behavior)
      {
        case EntityAIBehavior::Idle:
        {
          if (timeSinceBehaviorChanged >= idleTime)
          {
            aiComponent.behavior = EntityAIBehavior::Wander;
            aiComponent.behaviorChangedTime = currentTime;

            WanderAiComponent wanderAiComponent
            {
              .movingDirection = GetRandomDirection(),
              .lastPosition = transform.position
            };

            ecsRegistry.emplace<WanderAiComponent>(entity, wanderAiComponent);
          }

          break;
        }
        case EntityAIBehavior::Wander:
        {
          if (timeSinceBehaviorChanged >= wanderTime)
          {
            aiComponent.behavior = EntityAIBehavior::Idle;
            aiComponent.behaviorChangedTime = currentTime;

            physicsBody.velocity = glm::vec3(0.0f, 0.0f, physicsBody.velocity.z);

            ecsRegistry.remove<WanderAiComponent>(entity);
          }

          break;
        }
      }

      switch (aiComponent.behavior)
      {
        case EntityAIBehavior::Idle:
        {
          // Nothing to do here

          break;
        }
        case EntityAIBehavior::Wander:
        {
          WanderAiComponent wanderAiComponent = ecsRegistry.get<WanderAiComponent>(entity);

          static const float speed = 4.0f;
          physicsBody.velocity = glm::vec3(wanderAiComponent.movingDirection * speed, physicsBody.velocity.z);

          // Try to jump
          if (physicsBody.isGrounded && physicsBody.horizontalCollision && aiComponent.behaviorChangedTime < currentTime)
          {
            physicsBody.velocity.z += jumpVelocity;
          }

          wanderAiComponent.lastPosition = transform.position;

          break;
        }
      }
    }
  }


  glm::vec2 EntityAiModule::GetRandomDirection()
  {
    return glm::circularRand(1.0f);
  }
}
