#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <unordered_map>

#include "Render/shader.h"
#include "Render/texture.h"
#include "glad/glad.h"

#define GL_MAT_DATA_BUF 1

enum MaterialType { PHONG = 0, PBR = 1, CUSTOM = 2 };

enum MaterialFlags {
  HAS_ALBEDO_MAP = 1 << 0,
  HAS_NORMAL_MAP = 1 << 1,
  HAS_METALLIC_MAP = 1 << 2,
  HAS_ROUGHNESS_MAP = 1 << 3,
  HAS_METALIC_ROUGHNESS_MAP = 1 << 4,
  HAS_AO_MAP = 1 << 5,
  HAS_EMISSIVE_MAP = 1 << 6,
  HAS_SPECULAR_MAP = 1 << 7,
  HAS_HEIGHT_MAP = 1 << 8,

  HAS_ALBEDO_COLOR = 1 << 9,
  HAS_SPECULAR_COLOR = 1 << 10,
  HAS_AO_COLOR = 1 << 11,
  HAS_EMISSIVE_COLOR = 1 << 12,

  IS_TRANSPARENT = 1 << 16,
  IS_DOUBLE_SIDED = 1 << 17,
  IS_REFLECTIVE = 1 << 18,

  MATERIAL_TYPE_MASK = 3 << 24,
  MATERIAL_TYPE_PHONG = 0 << 24,
  MATERIAL_TYPE_PBR = 1 << 24,
  MATERIAL_TYPE_UNLIT = 2 << 24,
};

class Material {
 public:
  struct Data {
    int flags;
    float shininess = 12;
    float metallness;
    float roghness;
    glm::vec4 albedo;
    glm::vec4 specular;
    glm::vec4 ambient;
  };

  Material() = default;
  Material(Data data);

  std::shared_ptr<Shader> shader;

  void Bind(Shader& shader);
  bool Empty();

  void setUBOdata(Data& data);
  void loadUBO();
  void clearUBO();

  void setName(const std::string& name);
  void setType(MaterialType type);
  void setNewColor(const std::string& name, const glm::vec4& color);
  void setNewMap(const std::string& name, const std::shared_ptr<Texture> texture);
  void setNewFloat(const std::string& name, float num);
  void setNewInt(const std::string& name, int num);
  void setNewVector(const std::string& name, const glm::vec3& vector);
  void setNewFlag(const std::string& name, const bool flag);

  bool hasMap(const std::string& name);
  bool hasColor(const std::string& name);
  bool hasFloat(const std::string& name);
  bool hasInt(const std::string& name);
  bool hasVector(const std::string& name);
  bool hasFlag(const std::string& name);

 private:
  std::string m_name;
  Data m_data;
  GLuint m_UBO_ID = 0;

  std::unordered_map<std::string, glm::vec3> vectors;
  std::unordered_map<std::string, bool> flags;
  std::unordered_map<std::string, glm::vec4> colors;
  std::unordered_map<std::string, float> floats;
  std::unordered_map<std::string, int> ints;
  std::unordered_map<std::string, std::shared_ptr<Texture>> textures;

  MaterialType m_type = PHONG;
};
