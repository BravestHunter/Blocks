#include "camera.hpp"

const float Camera::defaultYaw = 90.0f;
const float Camera::defaultPitch = 0.0f;
const float Camera::defaultSpeed = 32.0f;
const float Camera::defaultSensitivity = 0.1f;
const float Camera::defaultZoom = 45.0f;


Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : forward_(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(defaultSpeed), MouseSensitivity(defaultSensitivity), Zoom(defaultZoom)
{
  position_ = position;
  WorldUp = up;
  Yaw = yaw;
  Pitch = pitch;
  UpdateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : forward_(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(defaultSpeed), MouseSensitivity(defaultSensitivity), Zoom(defaultZoom)
{
  position_ = glm::vec3(posX, posY, posZ);
  WorldUp = glm::vec3(upX, upY, upZ);
  Yaw = yaw;
  Pitch = pitch;
  UpdateCameraVectors();
}


glm::mat4 Camera::GetViewMatrix()
{
  return glm::lookAt(position_, position_ + forward_, up_);
}

float Camera::GetZoom()
{
  return Zoom;
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
  float velocity = MovementSpeed * deltaTime;
  if (direction == FORWARD)
    position_ += forward_ * velocity;
  if (direction == BACKWARD)
    position_ -= forward_ * velocity;
  if (direction == LEFT)
    position_ -= right_ * velocity;
  if (direction == RIGHT)
    position_ += right_ * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
  xoffset *= MouseSensitivity;
  yoffset *= MouseSensitivity;

  Yaw += xoffset;
  Pitch += yoffset;

  if (constrainPitch)
  {
    if (Pitch > 89.0f)
      Pitch = 89.0f;
    if (Pitch < -89.0f)
      Pitch = -89.0f;
  }

  UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
  Zoom -= (float)yoffset;
  if (Zoom < 1.0f)
    Zoom = 1.0f;
  if (Zoom > 45.0f)
    Zoom = 45.0f;
}


glm::vec3 Camera::GetPosition()
{
  return position_;
}

glm::vec3 Camera::GetForward()
{
  return right_;
}


void Camera::UpdateCameraVectors()
{
  glm::vec3 forward;
  forward.x = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  forward.y = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  forward.z = sin(glm::radians(Pitch));
  forward_ = glm::normalize(forward);
  
  right_ = glm::normalize(glm::cross(forward_, WorldUp));
  up_ = glm::normalize(glm::cross(right_, forward_));
}
