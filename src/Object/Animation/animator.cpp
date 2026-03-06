#include "animator.h"

#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_common.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/quaternion.hpp"

glm::mat4 AnimationChannel::interpolatePosition(float currentTime) {
  if (positions.empty()) return glm::translate(glm::mat4(1.0), glm::vec3(0.0));

  if (positions.size() == 1) {
    return glm::translate(glm::mat4(1.0), positions[0].value);
  }
  int index = 0;
  for (int i = 0; i < positions.size() - 1; i++) {
    if (currentTime < positions[i + 1].time) {
      index = i;
      break;
    }
  }

  int next = index + 1;

  float t1 = positions[index].time;
  float t2 = positions[next].time;
  float factor = (currentTime - t1) / (t2 - t1);

  glm::vec3 p1 = positions[index].value;
  glm::vec3 p2 = positions[next].value;

  glm::vec3 result =
      glm::vec3(p1.x + (p2.x - p1.x) * factor, p1.y + (p2.y - p1.y) * factor, p1.z + (p2.z - p1.z) * factor);
  return glm::translate(glm::mat4(1.0), result);
}

glm::mat4 AnimationChannel::interpolateScaling(float currentTime) {
  if (scales.empty()) return glm::translate(glm::mat4(1.0), glm::vec3(1.0));
  if (scales.size() == 1) {
    return glm::scale(glm::mat4(1.0), scales[0].value);
  }

  int index = 0;
  for (int i = 0; i < scales.size() - 1; i++) {
    if (currentTime < scales[i + 1].time) {
      index = i;
      break;
    }
  }

  int next = index + 1;

  float t1 = scales[index].time;
  float t2 = scales[next].time;
  float factor = (currentTime - t1) / (t2 - t1);

  glm::vec3 s1 = scales[index].value;
  glm::vec3 s2 = scales[next].value;

  glm::vec3 result =
      glm::vec3(s1.x + (s2.x - s1.x) * factor, s1.y + (s2.y - s1.y) * factor, s1.z + (s2.z - s1.z) * factor);
  return glm::scale(glm::mat4(1.0), result);
}

glm::mat4 AnimationChannel::interpolateRotation(float currentTime) {
  if (rotations.empty()) return glm::mat4_cast(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
  if (rotations.size() == 1) {
    return glm::mat4_cast(rotations[0].value);
  }

  int index = 0;
  for (int i = 0; i < rotations.size() - 1; i++) {
    if (currentTime < rotations[i + 1].time) {
      index = i;
      break;
    }
  }

  int next = index + 1;

  float t1 = rotations[index].time;
  float t2 = rotations[next].time;
  float factor = (currentTime - t1) / (t2 - t1);

  glm::quat q1 = rotations[index].value;
  glm::quat q2 = rotations[next].value;

  glm::quat result = glm::slerp(q1, q2, factor);
  result = glm::normalize(result);

  return glm::mat4_cast(result);
}
