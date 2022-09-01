#include "collisions_api.hpp"


namespace blocks
{
  RayIntersectionPoint CheckCollision(const Ray& ray, const AABB& bounds)
  {
    glm::vec3 invD = glm::vec3(1.0f / ray.direction.x, 1.0f / ray.direction.y, 1.0f / ray.direction.z);
    glm::vec3 t0s = (bounds.lowBorder - ray.origin) * invD;
    glm::vec3 t1s = (bounds.highBorder - ray.origin) * invD;

    glm::vec3 tsmaller = min(t0s, t1s);
    glm::vec3 tbigger = max(t0s, t1s);

    float tmin = fmax(tsmaller[0], fmax(tsmaller[1], tsmaller[2]));
    float tmax = fmin(tbigger[0], fmin(tbigger[1], tbigger[2]));

    RayIntersectionPoint result;
    if (tmin < tmax)
    {
      result.distance = tmin;
      result.point = ray.origin + tmin * ray.direction;
    }

    return result;
  }

  bool CheckCollision(const AABB& bounds1, const AABB& bounds2)
  {
    return
      bounds1.highBorder.x >= bounds2.lowBorder.x && bounds2.highBorder.x >= bounds1.lowBorder.x &&
      bounds1.highBorder.y >= bounds2.lowBorder.y && bounds2.highBorder.y >= bounds1.lowBorder.y &&
      bounds1.highBorder.z >= bounds2.lowBorder.z && bounds2.highBorder.z >= bounds1.lowBorder.z;
  }
}
