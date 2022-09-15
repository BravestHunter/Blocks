#include "camera.hpp"


namespace blocks
{
  const float Camera::defaultYaw = 90.0f;
  const float Camera::defaultPitch = 0.0f;
  const glm::vec3 Camera::WorldUp = glm::vec3(0.0f, 0.0f, 1.0f);


  Camera::Camera(glm::vec3 position, float yaw, float pitch) : position_(position), yaw_(yaw), pitch_(pitch), forward_(glm::vec3(0.0f, 0.0f, -1.0f))
  {
    RecalculateDirectionVectors();
  }


  void Camera::RecalculateDirectionVectors()
  {
    glm::vec3 forward;
    forward.x = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    forward.y = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    forward.z = sin(glm::radians(-pitch_));
    forward_ = glm::normalize(forward);

    right_ = glm::normalize(glm::cross(forward_, WorldUp));
    up_ = glm::normalize(glm::cross(right_, forward_));
  }
}
