#pragma once

#include <memory.h>

#include <memory>
#include <string>

#include "Object/mesh3D.h"

class SkyBox {
 public:
  SkyBox() = default;
  SkyBox(const std::string& pathToDir, std::shared_ptr<Shader> shader);

  void Bind();
  void Draw();

 private:
  std::string m_name;
  Mesh m_cube;
  std::shared_ptr<Texture> m_cubeMap;
  std::shared_ptr<Shader> m_shader;
};
