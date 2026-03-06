#pragma once

#include <glad/glad.h>

#include <string>

#include "external/stb_image.h"

class Texture {
 public:
  enum class TexType { CUBEMAP, BASIC };

  Texture() = default;
  Texture(const std::string& path, Texture::TexType type = Texture::TexType::BASIC, bool flipUVs = false);

  void Bind(unsigned int slot = 0);
  void Unbind();

  void setType(TexType type) { m_type = type; }

 private:
  unsigned int m_textureID;
  TexType m_type;
  int m_iWidth, m_iHeight, m_iNumOfComponents;
};

void unbindTextures();
