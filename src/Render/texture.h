#pragma once

#include <glad/glad.h>
#include <memory>
#include <string>
#include <unordered_map>

#include "external/stb_image.h"

class Texture {
public:
  enum class TexType { CUBEMAP, BASIC };

  Texture() = default;
  Texture(const std::string &path,
          Texture::TexType type = Texture::TexType::BASIC,
          bool flipUVs = false);

  void Bind(unsigned int slot = 0);
  void Unbind();

  void setType(TexType type) { m_type = type; }

private:
  unsigned int m_textureID;
  TexType m_type;
  int m_iWidth, m_iHeight, m_iNumOfComponents;
};

void unbindTextures();

class TextureManager {
public:
  static std::shared_ptr<Texture>
  getTexture(const std::string &path,
             const Texture::TexType type = Texture::TexType::BASIC,
             bool flipUVs = false) {

    auto it = cache.find(path);
    if (it != cache.end()) {
      return it->second;
    }

    auto texture = std::make_shared<Texture>(path, type);
    cache[path] = texture;
    return texture;
  }

  static void clear() { cache.clear(); }

private:
  static inline std::unordered_map<std::string, std::shared_ptr<Texture>> cache;
};
