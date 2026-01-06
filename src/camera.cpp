#include "camera.h"
#include <SDL3/SDL_timer.h>

float lastFrame = 0.f;

Camera::Camera(int width, int height, glm::vec3 position, glm::vec3 direction,
               bool projMode) {
  pitch = 0.f;
  yaw = -90.f;
  this->width = width;
  this->height = height;
  position.y = -position.y;
  this->position = position;
  this->direction = glm::normalize(direction);
  FOVdeg = 90.0f;
  up = glm::vec3(0.0f, 1.f, 0.0f);
  nearPlane = 0.1f;
  farPlane = 10000.f;
  proj = glm::mat4(1.f);
  view = glm::mat4(1.f);
  proj_mode = projMode;
  if (proj_mode == PERSPECTIVE)
    proj = glm::perspective(glm::radians(FOVdeg), (float)width / (float)height,
                            nearPlane, farPlane);
  else
    proj = glm::ortho(glm::radians(FOVdeg), (float)width / (float)height,
                      nearPlane, farPlane);
}

void Camera::updateCam(Shader &shader) {

  glm::vec3 camRight = glm::normalize(glm::cross(up, direction));
  glm::vec3 camUp = glm::normalize(glm::cross(camRight, direction));

  view = glm::lookAt(position, position + direction, camUp);

  shader.setUniformMatrix4f("u_PM", proj);
  shader.setUniformMatrix4f("u_VM", view);
}

void Camera::updateCamSkyBox(Shader &shader) {
  glm::vec3 camRight = glm::normalize(glm::cross(up, direction));
  glm::vec3 camUp = glm::normalize(glm::cross(camRight, direction));

  view =
      glm::mat4(glm::mat3(glm::lookAt(position, position + direction, camUp)));

  shader.setUniformMatrix4f("u_PM", proj);
  shader.setUniformMatrix4f("u_VM", view);
}
void Camera::changeSize(int width, int height) {
  this->width = width;
  this->height = height;
}
