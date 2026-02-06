#include "Math/mathUtils.h"

#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/quaternion.hpp"

namespace Math {
glm::mat4 calculateTransformMat4(glm::vec3 position, glm::vec3 scale, glm::quat rotation) {
  glm::mat4 scale_mat = glm::scale(glm::mat4(1.f), scale);
  glm::mat4 rotation_mat = glm::mat4_cast(rotation);
  glm::mat4 translation_mat = glm::translate(glm::mat4(1.f), position);

  return translation_mat * rotation_mat * scale_mat;
}

TransformProps decomposeTransformM(glm::mat4 m) {
  glm::vec3 position = glm::vec3(m[3]);

  glm::vec3 scale;
  scale.x = glm::length(glm::vec3(m[0]));
  scale.y = glm::length(glm::vec3(m[1]));
  scale.z = glm::length(glm::vec3(m[2]));

  glm::mat3 rotMat(glm::normalize(glm::vec3(m[0])), glm::normalize(glm::vec3(m[1])), glm::normalize(glm::vec3(m[2])));
  glm::quat rotation = glm::quat_cast(rotMat);
  return {position, rotation, scale};
}
glm::mat4 TransformProps::calculateTransform() const { return Math::calculateTransformMat4(position, scale, rotation); }
void TransformProps::decompose(glm::mat4 m) {
  TransformProps tmp = decomposeTransformM(m);
  position = tmp.position;
  rotation = tmp.rotation;
  scale = tmp.scale;
}

}  // namespace Math
