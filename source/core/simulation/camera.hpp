#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace blocks
{
  class Camera
  {
  private:
    static const float defaultYaw;
    static const float defaultPitch;
    static const float defaultSpeed;
    static const float defaultSensitivity;

    static const glm::vec3 WorldUp;

  public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), float yaw = defaultYaw, float pitch = defaultPitch);

    inline glm::mat4 GetViewMatrix() const;

    inline glm::vec3 GetPosition() const;
    inline float GetYaw() const;
    inline float GetPitch() const;
    inline glm::vec3 GetForward() const;
    inline glm::vec3 GetRight() const;
    inline glm::vec3 GetUp() const;

    inline void SetPosition(glm::vec3 position);
    inline void SetYaw(float yaw);
    inline void SetPitch(float pitch);

  private:
    glm::vec3 position_;
    float yaw_;
    float pitch_;
    glm::vec3 forward_;
    glm::vec3 up_;
    glm::vec3 right_;

    void RecalculateDirectionVectors();
  };


  glm::mat4 Camera::GetViewMatrix() const
  {
    return glm::lookAt(position_, position_ + forward_, up_);
  }


  glm::vec3 Camera::GetPosition() const
  {
    return position_;
  }

  float Camera::GetYaw() const
  {
    return yaw_;
  }

  float Camera::GetPitch() const
  {
    return pitch_;
  }

  glm::vec3 Camera::GetForward() const
  {
    return forward_;
  }

  glm::vec3 Camera::GetRight() const
  {
    return right_;
  }

  glm::vec3 Camera::GetUp() const
  {
    return up_;
  }


  void Camera::SetPosition(glm::vec3 position)
  {
    position_ = position;
  }

  inline void Camera::SetYaw(float yaw)
  {
    yaw_ = yaw;

    RecalculateDirectionVectors();
  }

  inline void Camera::SetPitch(float pitch)
  {
    if (pitch > 89.0f)
    {
      pitch_ = 89.0f;
    }
    else if (pitch < -89.0f)
    {
      pitch_ = -89.0f;
    }
    else
    {
      pitch_ = pitch;
    }

    RecalculateDirectionVectors();
  }
}
