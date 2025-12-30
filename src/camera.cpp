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

void Camera::updateCam(int shaderID) {

  glm::vec3 camRight = glm::normalize(glm::cross(up, direction));
  glm::vec3 camUp = glm::normalize(glm::cross(camRight, direction));

  /*float deltaTime = 0.0f; // Time between current frame and last frame
  // std::clog << direction.x << " " << direction.y << " " << direction.z <<
  // std::endl;

  float currentFrame = SDL_GetTicks();
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;
  float speed = 0.005f * deltaTime;

  // std::clog << speed << std::endl;
  if (mov_forward)
    position += direction * speed;
  if (mov_back)
    position -= direction * speed;
  if (mov_left)
    position += glm::normalize(glm::cross(camUp, direction)) * speed;
  if (mov_right)
    position -= glm::normalize(glm::cross(camUp, direction)) * speed;
  if (mov_up)
    position.y -= speed;
  if (mov_down)
    position.y += speed;

  if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;

  float sens = 0.0005f;
  direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch)) * sens;
  direction.y = sin(glm::radians(pitch)) * sens;
  direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch)) * sens;
  direction = glm::normalize(direction);
*/
  view = glm::lookAt(position, position + direction, camUp);

  int proj_u = glGetUniformLocation(shaderID, "u_PM");
  glUniformMatrix4fv(proj_u, 1, GL_FALSE, glm::value_ptr(proj));
  int view_u = glGetUniformLocation(shaderID, "u_VM");
  glUniformMatrix4fv(view_u, 1, GL_FALSE, glm::value_ptr(view));
}

void Camera::changeSize(int width, int height) {
  this->width = width;
  this->height = height;
}
