#include "material.h"

void Material::Bind(Shader &shader) {

  // Bind all properties
  for (const auto &[name, value] : vectors) {
    shader.setUniformVec3f(name, value);
  }

  for (const auto &[name, value] : colors) {
    shader.setUniformVec4f(name, value);
  }

  for (const auto &[name, value] : floats) {
    shader.setUniformFloat(name, value);
  }

  for (const auto &[name, value] : ints) {
    shader.setUniformInt(name, value);
  }

  for (const auto &[name, value] : flags) {
    shader.setUniformBool(name, value);
  }

  GLint currentTexUnit;
  glGetIntegerv(GL_ACTIVE_TEXTURE, &currentTexUnit);

  int textureSlot = 0;
  for (const auto &[name, texture] : textures) {
    texture->Bind(textureSlot);
    shader.setUniformInt(name, textureSlot);
    textureSlot++;
  }

  // Restore previous texture unit
  glActiveTexture(currentTexUnit);
}

bool Material::Empty() {
  return (vectors.empty() && colors.empty() && floats.empty() &&
          flags.empty() && textures.empty() && ints.empty());
}

void Material::setType(MaterialType type) {
  setNewInt("u_MatType", type);
  m_type = type;
}

void Material::setNewColor(const std::string &name, const glm::vec4 &color) {
  colors[name] = color;
}

void Material::setNewMap(const std::string &name,
                         std::shared_ptr<Texture> texture) {
  textures[name] = texture;
}

void Material::setNewFloat(const std::string &name, float num) {
  floats[name] = num;
}

void Material::setNewInt(const std::string &name, int num) { ints[name] = num; }

void Material::setNewVector(const std::string &name, const glm::vec3 &vector) {
  vectors[name] = vector;
}

void Material::setNewFlag(const std::string &name, const bool flag) {
  flags[name] = flag;
}
