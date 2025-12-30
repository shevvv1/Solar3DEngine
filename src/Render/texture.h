#pragma once

#include <glad/glad.h>
#include <memory>
#include <string>
#include <unordered_map>

#include "external/stb_image.h"

class Texture {
private:
  unsigned int textureID;
  int iWidth, iHeight, iNumOfComponents;
  std::string type;

public:
  std::string path;
  Texture(const std::string &filepath);
  // ~Texture();

  std::string GetType() { return type; }
  void Bind(unsigned int slot = 0);
  void Unbind();
};

void unbindTextures();

class TextureManager {
public:
  static std::shared_ptr<Texture> getTexture(const std::string &path) {
    auto it = cache.find(path);
    if (it != cache.end()) {
      return it->second;
    }

    auto texture = std::make_shared<Texture>(path);
    cache[path] = texture;
    return texture;
  }

  static void clear() { cache.clear(); }

private:
  static inline std::unordered_map<std::string, std::shared_ptr<Texture>> cache;
};
