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

  void setPosition(glm::vec3 position);
  void setDirection(glm::vec3 direction);
  void setDrawDistant(float nearPlane, float farPlane);
  void setFOV(float FOVdeg);
  void setProgMode(bool mode);
  void setSize(int width, int height);
  void setYawPitch(glm::vec2 yawPitch);

  void offsetPosition(glm::vec3 offset);
  void offsetPosition(float x = 0, float y = 0, float z = 0);
  void offsetYawPitch(glm::vec2 yawPitch);
  void offsetYawPitch(float yaw, float pitch);

  glm::vec3 GetPosition() { return m_position; }
  glm::vec3 GetDirection() { return m_direction; }
  glm::vec3 GetUpVec() { return m_up; }
  glm::vec3 GetRightVec() { return m_right; }

  float yaw, pitch;

private:
  int m_width, m_height;

  float m_nearPlane, m_farPlane;
  float m_FOV;
  bool m_proj_mode;

  glm::vec3 m_right, m_up, m_position, m_direction;
  glm::mat4 m_proj, m_view;
};
