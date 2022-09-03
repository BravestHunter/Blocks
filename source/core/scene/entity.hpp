#pragma once

#include <glm/glm.hpp>

#include "geometry/aabb.hpp"


namespace blocks
{
  class Entity
  {
  public:
    Entity(glm::vec3 position, AABB aabb);

    inline glm::vec3 GetPosition()const;
    inline glm::vec3 GetVelocity() const;
    inline AABB GetAABB() const;
    inline bool IsGrounded() const;

    inline void SetPosition(glm::vec3 position);
    inline void SetVelocity(glm::vec3 velocity);
    inline void SetGrounded(bool isGrounded);

  private:
    glm::vec3 position_;
    glm::vec3 velocity_;
    AABB aabb_;
    bool isGrounded_;
  };


  glm::vec3 Entity::GetPosition() const
  {
    return position_;
  }

  glm::vec3 Entity::GetVelocity() const
  {
    return velocity_;
  }

  AABB Entity::GetAABB() const
  {
    return aabb_;
  }

  bool Entity::IsGrounded() const
  {
    return isGrounded_;
  }


  void Entity::SetPosition(glm::vec3 position)
  {
    position_ = position;
  }

  void Entity::SetVelocity(glm::vec3 velocity)
  {
    velocity_ = velocity;
  }

  void Entity::SetGrounded(bool isGrounded)
  {
    isGrounded_ = isGrounded;
  }
}
