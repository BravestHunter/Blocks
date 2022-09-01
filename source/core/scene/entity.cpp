#include "entity.hpp"


namespace blocks
{
  Entity::Entity(glm::vec3 position, AABB aabb) : position_(position), velocity_(0), aabb_(aabb)
  {
  }
}
