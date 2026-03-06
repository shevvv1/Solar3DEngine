#include "cacheManager.h"

std::shared_ptr<Texture> TextureManager::getTexture(const std::string& path, const Texture::TexType type,
                                                    bool flipUVs) {
  auto it = cache.find(path);
  if (it != cache.end()) {
    return it->second;
  }
  auto texture = std::make_shared<Texture>(path, type, flipUVs);
  cache[path] = texture;
  return texture;
}
