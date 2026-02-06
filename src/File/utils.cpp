#include "File/utils.h"

#include <filesystem>
#include <string>

namespace FileUtils {
std::string ExtractFilename(const char* path) {
  std::string name = std::filesystem::path(path).filename().string();
  if (name == path) {
    std::string separator = "/\\";
    name = name.substr(name.find_last_of(separator) + 1);
    return name;
  }
  return name;
}

std::string ExtractRootDir(const char* path) { return std::filesystem::path(path).parent_path().string(); }
std::string ReplaceExtension(const std::string& file, const std::string& newExt) {
  return std::filesystem::path(file).replace_extension(newExt).string();
}

std::string convertTextureFilePath(const char* texturePath, const char* RootDir) {
  std::filesystem::path root(RootDir);
  std::filesystem::path textureDir = root / "textures/";

  std::string texName = ExtractFilename(texturePath);

  // пробуем разные варианты
  std::string candidates[6] = {texName, ReplaceExtension(texName, ".png"), ReplaceExtension(texName, ".jpg"),
                               ReplaceExtension(texName, ".jpeg"), ReplaceExtension(texName, ".tga")};
  std::string finalPath;

  for (auto& c : candidates) {
    auto p = textureDir / c;
    auto p1 = root / c;
    if (std::filesystem::exists(p)) {
      finalPath = p.string();
      break;
    } else if (std::filesystem::exists(p1)) {
      finalPath = p1.string();
      break;
    }
  }
  return finalPath;
}
}  // namespace FileUtils
