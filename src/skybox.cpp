#include "skybox.h"
#include "Render/mesh.h"
#include "Render/texture.h"

SkyBox::SkyBox(const std::string &pathToDir) {
  m_cubeMap = TextureManager::getTexture(pathToDir, Texture::TexType::CUBEMAP);

  std::vector<VertexP> skyboxVertices = {
      {glm::vec3(-1.0f, -1.0f, 1.0f)}, {glm::vec3(1.0f, -1.0f, 1.0f)},
      {glm::vec3(1.0f, -1.0f, -1.0f)}, {glm::vec3(-1.0f, -1.0f, -1.0f)},
      {glm::vec3(-1.0f, 1.0f, 1.0f)},  {glm::vec3(1.0f, 1.0f, 1.0f)},
      {glm::vec3(1.0f, 1.0f, -1.0f)},  {glm::vec3(-1.0f, 1.0f, -1.0f)}};
  std::vector<unsigned int> skyboxIndices = {// Right
                                             1, 2, 6, 6, 5, 1,
                                             // Left
                                             0, 4, 7, 7, 3, 0,
                                             // Top
                                             4, 5, 6, 6, 7, 4,
                                             // Bottom
                                             0, 3, 2, 2, 1, 0,
                                             // Back
                                             0, 1, 5, 5, 4, 0,
                                             // Front
                                             3, 7, 6, 6, 2, 3};
  Mesh<VertexP> mesh(skyboxVertices, skyboxIndices);
  m_cube = mesh;
}

void SkyBox::Bind() { m_cubeMap->Bind(); }

void SkyBox::Draw(Shader &shader) {
  m_cubeMap->Bind(0);
  shader.setUniformInt("skybox", 0);
  m_cube.Draw(shader);
}
