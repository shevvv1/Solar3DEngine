#pragma once

#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Render/shader.h"

#define ORTHO 0
#define PERSPECTIVE 1

class Camera {
public:
  Camera() {}
  Camera(int width, int height, glm::vec3 position, glm::vec3 direction,
         bool projMode = PERSPECTIVE);

  void updateCam(Shader &shader);
  void updateCamSkyBox(Shader &shader);

  void changeDrawDistant(float nearPlane, float farPlane);
  void changeFOV(float FOVdeg);
  void changeProgMode(bool mode);
  void changeSize(int width, int height);

  glm::vec3 GetPosition() { return position; }
  glm::vec3 GetDirection() { return direction; }

private:
  int width, height;

  bool mov_forward, mov_back, mov_left, mov_right, mov_up, mov_down;

  float yaw, pitch;

  float nearPlane, farPlane;
  float FOVdeg;
  bool proj_mode;

  glm::vec3 position, up, direction;
  glm::mat4 proj, view;
};
