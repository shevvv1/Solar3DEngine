#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "Render/shader.h"
#include "Render/texture.h"

class TextureManager {
 public:
  static std::shared_ptr<Texture> getTexture(const std::string& path,
                                             const Texture::TexType type = Texture::TexType::BASIC,
                                             bool flipUVs = false);

  static void clear() { cache.clear(); }

 private:
  static inline std::unordered_map<std::string, std::shared_ptr<Texture>> cache;
};

class ShaderManager {
 public:
 private:
  static inline std::unordered_map<std::string, Shader> cache;
};
