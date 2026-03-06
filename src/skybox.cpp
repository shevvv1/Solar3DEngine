#include "skybox.h"

#include <memory>

#include "CacheManager/cacheManager.h"
#include "Object/mesh3D.h"

SkyBox::SkyBox(const std::string& pathToDir, std::shared_ptr<Shader> shader) {
  m_cubeMap = TextureManager::getTexture(pathToDir, Texture::TexType::CUBEMAP, false);

  std::vector<Vertex> skyboxVertices = {{glm::vec3(-1.0f, -1.0f, 1.0f)}, {glm::vec3(1.0f, -1.0f, 1.0f)},
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

  m_name = "skybox";
  m_cube = Mesh(skyboxVertices, skyboxIndices, m_name, Mesh::Type::VERTEXP);
  m_cube.CreateVAO();
  m_shader = shader;
}

void SkyBox::Bind() { m_cubeMap->Bind(); }

void SkyBox::Draw() {
  m_cubeMap->Bind(0);
  m_shader->setUniformInt("skybox", 0);
  m_cube.Draw(m_shader, DrawMethod::TRIANGLE_SINGLE);
}
