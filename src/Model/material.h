#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "Render/shader.h"
#include "Render/texture.h"

enum class MaterialType { PHONG, PBR, CUSTOM };

class Material {
public:
  Material() = default;
  Material(Shader &shader, MaterialType type);

  std::shared_ptr<Shader> shader;
  MaterialType type;

  void Bind();

  void setNewColor(const std::string &name, const glm::vec4 &color);
  void setNewMap(const std::string &name,
                 const std::shared_ptr<Texture> texture);
  void setNewFloat(const std::string &name, float num);
  void setNewVector(const std::string &name, const glm::vec3 &vector);
  void setNewFlag(const std::string &name, const bool flag);

private:
  std::unordered_map<std::string, glm::vec3> vectors;
  std::unordered_map<std::string, bool> flags;
  std::unordered_map<std::string, glm::vec4> colors;
  std::unordered_map<std::string, float> floats;
  std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
};
