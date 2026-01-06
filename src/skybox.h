#pragma once

#include "Render/mesh.h"
#include <memory.h>
#include <memory>
#include <string>

class SkyBox {
public:
  SkyBox() = default;
  SkyBox(const std::string &pathToDir);

  void Bind();
  void Draw(Shader &shader);

private:
  Mesh<VertexP> m_cube;
  std::shared_ptr<Texture> m_cubeMap;
};
