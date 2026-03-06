#include "material.h"

#include <string>

#include "glad/glad.h"

Material::Material(Data data) {
  m_data = data;
  loadUBO();
}

void Material::Bind(Shader& shader) {
  glBindBufferBase(GL_UNIFORM_BUFFER, GL_MAT_DATA_BUF, m_UBO_ID);

  for (const auto& [name, value] : vectors) {
    shader.setUniformVec3f(name, value);
  }

  for (const auto& [name, value] : colors) {
    shader.setUniformVec4f(name, value);
  }

  for (const auto& [name, value] : floats) {
    shader.setUniformFloat(name, value);
  }

  for (const auto& [name, value] : ints) {
    shader.setUniformInt(name, value);
  }

  for (const auto& [name, value] : flags) {
    shader.setUniformBool(name, value);
  }

  int textureSlot = 0;
  for (const auto& [name, texture] : textures) {
    texture->Bind(textureSlot);
    shader.setUniformInt(name, textureSlot);
    textureSlot++;
  }
}
bool Material::Empty() {
  return (vectors.empty() && colors.empty() && floats.empty() && flags.empty() && textures.empty() && ints.empty());
}

void Material::setUBOdata(Data& data) {
  m_data = data;
  loadUBO();
}

void Material::loadUBO() {
  clearUBO();
  glGenBuffers(1, &m_UBO_ID);
  glBindBuffer(GL_UNIFORM_BUFFER, m_UBO_ID);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(Material::Data), &m_data, GL_STATIC_DRAW);
}

void Material::clearUBO() {
  if (m_UBO_ID == 0) {
    return;
  }
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
  glDeleteBuffers(1, &m_UBO_ID);
}

void Material::setName(const std::string& name) { m_name = name; }

void Material::setType(MaterialType type) {
  setNewInt("u_MatType", type);
  m_type = type;
}

void Material::setNewColor(const std::string& name, const glm::vec4& color) { colors[name] = color; }

void Material::setNewMap(const std::string& name, std::shared_ptr<Texture> texture) { textures[name] = texture; }

void Material::setNewFloat(const std::string& name, float num) { floats[name] = num; }

void Material::setNewInt(const std::string& name, int num) { ints[name] = num; }

void Material::setNewVector(const std::string& name, const glm::vec3& vector) { vectors[name] = vector; }

void Material::setNewFlag(const std::string& name, const bool flag) { flags[name] = flag; }

bool Material::hasMap(const std::string& name) { return textures.find(name) != textures.end(); }
bool Material::hasColor(const std::string& name) { return colors.find(name) != colors.end(); }
bool Material::hasFloat(const std::string& name) { return floats.find(name) != floats.end(); }
bool Material::hasInt(const std::string& name) { return ints.find(name) != ints.end(); }
bool Material::hasVector(const std::string& name) { return vectors.find(name) != vectors.end(); }
bool Material::hasFlag(const std::string& name) { return flags.find(name) != flags.end(); }
