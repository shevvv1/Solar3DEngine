#include <iostream>

#include "Object/object3D.h"

SkinnedObject3D::SkinnedObject3D(std::string const& path, std::shared_ptr<Shader> shader, Mesh::Type meshType) {
  std::clog << "Loading skinned model:" << path << "\n";
  m_meshType = meshType;
  m_shader = shader;
  loadObject3D(path);
  Update();
}
