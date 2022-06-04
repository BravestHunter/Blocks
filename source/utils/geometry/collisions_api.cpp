#include "collisions_api.hpp"


namespace blocks
{
  RayIntersectionPoint CheckCollision(const Ray& ray, const AABB& bounds)
  {
    glm::vec3 invD = glm::vec3(1.0f / ray.direction.x, 1.0f / ray.direction.y, 1.0f / ray.direction.z);
    glm::vec3 t0s = (bounds.low - ray.origin) * invD;
    glm::vec3 t1s = (bounds.high - ray.origin) * invD;

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
      bounds1.high.x >= bounds2.low.x && bounds2.high.x >= bounds1.low.x &&
      bounds1.high.y >= bounds2.low.y && bounds2.high.y >= bounds1.low.y &&
      bounds1.high.z >= bounds2.low.z && bounds2.high.z >= bounds1.low.z;
  }
}
