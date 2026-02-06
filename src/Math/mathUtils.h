#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Math {
struct TransformProps {
  glm::vec3 position = glm::vec3(0.0);
  glm::quat rotation = glm::quat(1.0, 0.0, 0.0, 0.0);
  glm::vec3 scale = glm::vec3(1.0);

  glm::mat4 calculateTransform() const;
  void decompose(glm::mat4 m);
};

glm::mat4 calculateTransformMat4(glm::vec3 position, glm::vec3 scale, glm::quat rotation);
TransformProps decomposeTransformM(glm::mat4 m);
}  // namespace Math
