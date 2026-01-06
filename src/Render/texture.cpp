#define STB_IMAGE_IMPLEMENTATION
#include "texture.h"
#include "glad/glad.h"
#include <filesystem>
#include <iostream>
#include <map>
#include <string>

namespace fs = std::filesystem;

Texture::Texture(const std::string &path, Texture::TexType type) {

  stbi_set_flip_vertically_on_load(true);
  m_type = type;
  unsigned char *img_data;

  switch (m_type) {
  case TexType::BASIC:
    img_data =
        stbi_load(path.c_str(), &m_iWidth, &m_iHeight, &m_iNumOfComponents, 0);
    if (img_data) {
      GLenum format;
      if (m_iNumOfComponents == 1)
        format = GL_RED;
      else if (m_iNumOfComponents == 3)
        format = GL_RGB;
      else if (m_iNumOfComponents == 4)
        format = GL_RGBA;

      glGenTextures(1, &m_textureID);
      glBindTexture(GL_TEXTURE_2D, m_textureID);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, m_iWidth, m_iHeight, 0, format,
                   GL_UNSIGNED_BYTE, img_data);
      glGenerateMipmap(GL_TEXTURE_2D);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                      GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else
      std::cerr << "ERROR: Texture " << path << " don`t exist!" << std::endl;

    glBindTexture(GL_TEXTURE_2D, 0);
    break;
  case TexType::CUBEMAP:
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);

    fs::path dir_path = path;
    std::map<std::string, int> faceOrder = {
        {"right", 0}, {"left", 1}, {"top", 2},  {"bottom", 3},
        {"front", 4}, {"back", 5}, {"posx", 0}, {"negx", 1},
        {"posy", 2},  {"negy", 3}, {"posz", 4}, {"negz", 5}};

    int i = 0;
    for (const auto &entry : fs::directory_iterator(dir_path)) {

      img_data = stbi_load(entry.path().c_str(), &m_iWidth, &m_iHeight,
                           &m_iNumOfComponents, 0);
      if (img_data) {
        GLenum format;
        if (m_iNumOfComponents == 1)
          format = GL_RED;
        else if (m_iNumOfComponents == 3)
          format = GL_RGB;
        else if (m_iNumOfComponents == 4)
          format = GL_RGBA;

        int numOfFace = faceOrder[entry.path().stem()];
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + numOfFace, 0, format,
                     m_iWidth, m_iHeight, 0, format, GL_UNSIGNED_BYTE,
                     img_data);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,
                        GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,
                        GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R,
                        GL_CLAMP_TO_EDGE);
        i++;
        if (img_data)
          stbi_image_free(img_data);
      } else {
        std::cerr << "ERROR: Texture " << entry.path().string()
                  << " don`t exist!" << std::endl;

        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        break;
      }
    }
    break;
  }
}

/*Texture::~Texture(){
    glDeleteTextures(1,&textureID);
}*/

void Texture::Bind(unsigned int slot) {
  glActiveTexture(GL_TEXTURE0 + slot);
  switch (m_type) {
  case TexType::BASIC:
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    break;
  case TexType::CUBEMAP:
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
    break;
  }
}

void Texture::Unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

void unbindTextures() { glBindTexture(GL_TEXTURE_2D, 0); }
