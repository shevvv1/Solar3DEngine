#define STB_IMAGE_IMPLEMENTATION
#include "texture.h"
#include <iostream>

Texture::Texture(const std::string &filepath) {

  stbi_set_flip_vertically_on_load(true);
  this->type = type;
  this->path = filepath;

  unsigned char *img_data =
      stbi_load(filepath.c_str(), &iWidth, &iHeight, &iNumOfComponents, 0);
  if (img_data) {
    GLenum format;
    if (iNumOfComponents == 1)
      format = GL_RED;
    else if (iNumOfComponents == 3)
      format = GL_RGB;
    else if (iNumOfComponents == 4)
      format = GL_RGBA;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, iWidth, iHeight, 0, format,
                 GL_UNSIGNED_BYTE, img_data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
  } else
    std::cerr << "ERROR: Texture " << filepath << " don`t exist!" << std::endl;

  if (img_data)
    stbi_image_free(img_data);
}

/*Texture::~Texture(){
    glDeleteTextures(1,&textureID);
}*/

void Texture::Bind(unsigned int slot) {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, textureID);
  glEnable(GL_TEXTURE_2D);
}

void Texture::Unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

void unbindTextures() { glBindTexture(GL_TEXTURE_2D, 0); }
