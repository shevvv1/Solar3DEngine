#pragma once

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

namespace FileUtils {
std::string ExtractFilename(const char* path);
std::string ExtractRootDir(const char* path);

std::string ReplaceExtension(const std::string& file, const std::string& newExt);
std::string convertTextureFilePath(const char* texturePath, const char* RootDir);
}  // namespace FileUtils
