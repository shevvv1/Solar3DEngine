#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <unordered_map>

#include "Render/shader.h"
#include "Render/texture.h"

enum MaterialType { PHONG = 0, PBR = 1, CUSTOM = 2 };

class Material {
 public:
  Material() = default;

  std::shared_ptr<Shader> shader;

  void Bind(Shader& shader);
  bool Empty();

  void setType(MaterialType type);
  void setNewColor(const std::string& name, const glm::vec4& color);
  void setNewMap(const std::string& name, const std::shared_ptr<Texture> texture);
  void setNewFloat(const std::string& name, float num);
  void setNewInt(const std::string& name, int num);
  void setNewVector(const std::string& name, const glm::vec3& vector);
  void setNewFlag(const std::string& name, const bool flag);
  void setDoubleSide(bool isDoubleSide);

  bool isDoubleSide() { return m_doubleSided; }

 private:
  std::unordered_map<std::string, glm::vec3> vectors;
  std::unordered_map<std::string, bool> flags;
  std::unordered_map<std::string, glm::vec4> colors;
  std::unordered_map<std::string, float> floats;
  std::unordered_map<std::string, int> ints;
  std::unordered_map<std::string, std::shared_ptr<Texture>> textures;

  MaterialType m_type = PHONG;
  bool m_doubleSided = false;
};
