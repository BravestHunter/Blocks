#pragma once

#include "aabb.hpp"
#include "ray.hpp"
#include "ray_intersection_point.hpp"


RayIntersectionPoint CheckCollision(const Ray& ray, const AABB& bounds);
bool CheckCollision(const AABB& bounds1, const AABB& bounds2);
