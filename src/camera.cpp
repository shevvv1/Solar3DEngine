#include "camera.h"

#include <SDL3/SDL_timer.h>

#include "glm/geometric.hpp"

float lastFrame = 0.f;

Camera::Camera(int width, int height, glm::vec3 position, glm::vec3 direction, bool projMode) {
  m_direction = glm::normalize(direction);
  pitch = 0;
  yaw = -90;
  m_width = width;
  m_height = height;
  m_position = position;
  m_FOV = 90.0f;
  m_up = glm::vec3(0.0f, 1.f, 0.0f);
  m_nearPlane = 0.1f;
  m_farPlane = 10000.f;
  m_proj = glm::mat4(1.f);
  m_view = glm::mat4(1.f);
  m_proj_mode = projMode;
  if (m_proj_mode == PERSPECTIVE)
    m_proj = glm::perspective(glm::radians(m_FOV), (float)m_width / (float)m_height, m_nearPlane, m_farPlane);
  else
    m_proj = glm::ortho(glm::radians(m_FOV), (float)width / (float)height, m_nearPlane, m_farPlane);
  m_proj[1][1] *= -1;
}

void Camera::updateCam(Shader& shader) {
  m_right = glm::normalize(glm::cross(m_up, m_direction));
  glm::vec3 camUp = glm::normalize(glm::cross(m_right, m_direction));

  if (pitch > 89.0f) pitch = 89.0f;
  if (pitch < -89.0f) pitch = -89.0f;

  m_direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  m_direction.y = sin(glm::radians(pitch));
  m_direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  m_direction = glm::normalize(m_direction);

  m_view = glm::mat4(1.0f);
  m_view = glm::lookAt(m_position, m_position + m_direction, camUp);

  m_proj = glm::mat4(1.f);
  if (m_proj_mode == PERSPECTIVE)
    m_proj = glm::perspective(glm::radians(m_FOV), (float)m_width / (float)m_height, m_nearPlane, m_farPlane);
  else
    m_proj = glm::ortho(glm::radians(m_FOV), (float)m_width / (float)m_height, m_nearPlane, m_farPlane);
  m_proj[1][1] *= -1;

  shader.setUniformMatrix4f("u_PM", m_proj);
  shader.setUniformMatrix4f("u_VM", m_view);
}

void Camera::updateCamSkyBox(Shader& shader) {
  m_right = glm::normalize(glm::cross(m_up, m_direction));
  glm::vec3 camUp = glm::normalize(glm::cross(m_right, m_direction));
  m_view = glm::mat4(1.0f);
  m_view = glm::mat4(glm::mat3(glm::lookAt(m_position, m_position + m_direction, camUp)));
  m_proj = glm::mat4(1.f);
  if (m_proj_mode == PERSPECTIVE)
    m_proj = glm::perspective(glm::radians(m_FOV), (float)m_width / (float)m_height, m_nearPlane, m_farPlane);
  else
    m_proj = glm::ortho(glm::radians(m_FOV), (float)m_width / (float)m_height, m_nearPlane, m_farPlane);
  m_proj[1][1] *= -1;

  shader.setUniformMatrix4f("u_PM", m_proj);
  shader.setUniformMatrix4f("u_VM", m_view);
}

void Camera::setPosition(glm::vec3 position) { m_position = position; }

void Camera::setDirection(glm::vec3 direction) { m_direction = glm::normalize(direction); }

void Camera::setSize(int width, int height) {
  m_width = width;
  m_height = height;
}

void Camera::setFOV(float FOV) { m_FOV = FOV; }

void Camera::offsetPosition(glm::vec3 offset) { m_position += offset; }

void Camera::offsetPosition(float x, float y, float z) {
  m_position.x = x;
  m_position.y = y;
  m_position.z = z;
}

void Camera::setYawPitch(glm::vec2 yawPitch) {
  yaw = yawPitch.x;
  pitch = yawPitch.y;
}

void Camera::offsetYawPitch(glm::vec2 yawPitch) {
  yaw += yawPitch.x;
  pitch += yawPitch.y;
}

void Camera::offsetYawPitch(float yaw, float pitch) {
  this->yaw += yaw;
  this->pitch += pitch;
}
