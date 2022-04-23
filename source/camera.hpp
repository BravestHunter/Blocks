#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Camera
{
public:
  enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
  };

private:
  static const float defaultYaw;
  static const float defaultPitch;
  static const float defaultSpeed;
  static const float defaultSensitivity;
  static const float defaultZoom;

public:
  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f), float yaw = defaultYaw, float pitch = defaultPitch);
  Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

  glm::mat4 GetViewMatrix();
  float GetZoom();
  void ProcessKeyboard(Camera_Movement direction, float deltaTime);
  void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
  void ProcessMouseScroll(float yoffset);

  glm::vec3 GetPosition();
  glm::vec3 GetForward();

private:
  glm::vec3 position_;
  glm::vec3 forward_;
  glm::vec3 up_;
  glm::vec3 right_;
  glm::vec3 WorldUp;
  float Yaw;
  float Pitch;
  float MovementSpeed;
  float MouseSensitivity;
  float Zoom;

  void UpdateCameraVectors();
};
