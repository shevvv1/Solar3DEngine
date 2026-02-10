#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>
#include <unordered_map>
#include <vector>

struct Bone {
  std::string name;
  int node_indx;
  glm::mat4 offsetMatrix;
};

struct KeyVec3 {
  float time;
  glm::vec3 value;
};

struct KeyQuat {
  float time;
  glm::quat value;
};

struct AnimationChannel {
  std::vector<KeyVec3> positions;
  std::vector<KeyQuat> rotations;
  std::vector<KeyVec3> scales;

  glm::mat4 interpolatePosition(float currentTime);
  glm::mat4 interpolateRotation(float currentTime);
  glm::mat4 interpolateScaling(float currentTime);
  float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);
};

struct Animation {
  std::string name;
  float duration;
  float ticksPerSecond;
  std::unordered_map<std::string, AnimationChannel> channels;
};
