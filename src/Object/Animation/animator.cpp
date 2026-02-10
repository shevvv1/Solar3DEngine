#include "animator.h"

#include "glm/fwd.hpp"
#include "glm/gtc/quaternion.hpp"
glm::mat4 AnimationChannel::interpolatePosition(float currentTime) {
  if (1 == positions.size()) return glm::translate(glm::mat4(1.0f), positions[0].value);

  int p0Index;
  for (int index = 0; index < positions.size() - 1; ++index) {
    if (currentTime < positions[index + 1].time) p0Index = index;
  }
  int p1Index = p0Index + 1;
  float scaleFactor = GetScaleFactor(positions[p0Index].time, positions[p1Index].time, currentTime);
  glm::vec3 finalPosition = glm::mix(positions[p0Index].value, positions[p1Index].value, scaleFactor);
  return glm::translate(glm::mat4(1.0f), finalPosition);
}

glm::mat4 AnimationChannel::interpolateRotation(float currentTime) {
  if (1 == rotations.size()) {
    auto rotation = glm::normalize(rotations[0].value);
    return glm::mat4_cast(rotation);
  }

  int p0Index;
  for (int index = 0; index < rotations.size() - 1; ++index) {
    if (currentTime < rotations[index + 1].time) p0Index = index;
  }
  int p1Index = p0Index + 1;
  float scaleFactor = GetScaleFactor(rotations[p0Index].time, rotations[p1Index].time, currentTime);
  glm::quat finalRotation = glm::slerp(rotations[p0Index].value, rotations[p1Index].value, scaleFactor);
  finalRotation = glm::normalize(finalRotation);
  return glm::mat4_cast(finalRotation);
}

glm::mat4 AnimationChannel::interpolateScaling(float currentTime) {
  if (1 == scales.size()) return glm::scale(glm::mat4(1.0f), scales[0].value);

  int p0Index;
  for (int index = 0; index < scales.size() - 1; ++index) {
    if (currentTime < scales[index + 1].time) p0Index = index;
  }
  int p1Index = p0Index + 1;
  float scaleFactor = GetScaleFactor(scales[p0Index].time, scales[p1Index].time, currentTime);
  glm::vec3 finalScale = glm::mix(scales[p0Index].value, scales[p1Index].value, scaleFactor);
  return glm::scale(glm::mat4(1.0f), finalScale);
}

float AnimationChannel::GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime) {
  float scaleFactor = 0.0f;
  float midWayLength = animationTime - lastTimeStamp;
  float framesDiff = nextTimeStamp - lastTimeStamp;
  scaleFactor = midWayLength / framesDiff;
  return scaleFactor;
}
