#include "entity_ai_module.hpp"

#include "ecs/components/transform.hpp"
#include "ecs/components/physics_body.hpp"
#include "ecs/components/ai_component.hpp"


namespace blocks
{
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

      switch (aiComponent.behavior)
      {
        case EntityAIBehavior::Idle:
        {
          if (timeSinceBehaviorChanged >= 8.0f)
          {
            aiComponent.behavior = EntityAIBehavior::Wander;
            aiComponent.behaviorChangedTime = currentTime;

            WanderAiComponent wanderAiComponent
            {
              .movingDirection = GetRandomDirection()
            };

            ecsRegistry.emplace<WanderAiComponent>(entity, wanderAiComponent);
          }

          break;
        }
        case EntityAIBehavior::Wander:
        {
          if (timeSinceBehaviorChanged >= 4.0f)
          {
            aiComponent.behavior = EntityAIBehavior::Idle;
            aiComponent.behaviorChangedTime = currentTime;

            physicsBody.velocity = glm::vec3(0.0f, 0.0f, physicsBody.velocity.z);

            ecsRegistry.remove<WanderAiComponent>(entity);
          }
          else
          {
            WanderAiComponent wanderAiComponent = ecsRegistry.get<WanderAiComponent>(entity);

            static const float speed = 4.0f;
            physicsBody.velocity = glm::vec3(wanderAiComponent.movingDirection * speed, physicsBody.velocity.z);
          }

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
