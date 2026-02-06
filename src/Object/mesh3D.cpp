#include "mesh3D.h"

#include "glad/glad.h"

template <typename VertexType>
Mesh<VertexType>::Mesh() {
  setup_full_mesh();
}

template <typename VertexType>
Mesh<VertexType>::Mesh(std::vector<VertexType> vertices, std::vector<unsigned int> indices, Material material) {
  this->vertices = vertices;
  this->indices = indices;
  this->material = material;
  setup_full_mesh();
}

template <typename VertexType>
Mesh<VertexType>::Mesh(std::vector<VertexType> vertices, std::vector<unsigned int> indices) {
  this->vertices = vertices;
  this->indices = indices;
  setup_full_mesh();
}

/*Mesh::~Mesh(){
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&EBO);
    glDeleteVertexArrays(1,&VAO);
}*/

template <typename VertexType>
void Mesh<VertexType>::setup_full_mesh() {
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexType), vertices.data(), GL_STATIC_DRAW);

  if (!indices.empty()) {
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    size_t sizeEBO = sizeof(glm::vec3);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
  }

  const auto& layout = VertexType::layout();
  m_NumLayouts = layout.size();
  for (const auto& attr : layout) {
    glEnableVertexAttribArray(attr.index);
    glVertexAttribPointer(attr.index, attr.size, attr.type, attr.normalized, attr.stride,
                          reinterpret_cast<void*>(attr.offset));
  }
  glBindVertexArray(0);
}

template <typename VertexType>
void Mesh<VertexType>::makeInstanced(std::shared_ptr<std::vector<glm::mat4>> tranformMatArr) {
  if (tranformMatArr->empty()) {
    return;
  }
  m_instance_mat = tranformMatArr;
  glBindVertexArray(VAO);
  glGenBuffers(1, &instanceVBO);
  glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
  glBufferData(GL_ARRAY_BUFFER, m_instance_mat->size() * sizeof(glm::mat4), m_instance_mat->data(), GL_STATIC_DRAW);

  std::size_t vec4Size = sizeof(glm::vec4);
  glEnableVertexAttribArray(m_NumLayouts);
  glVertexAttribPointer(m_NumLayouts, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
  glEnableVertexAttribArray(m_NumLayouts + 1);
  glVertexAttribPointer(m_NumLayouts + 1, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
  glEnableVertexAttribArray(m_NumLayouts + 2);
  glVertexAttribPointer(m_NumLayouts + 2, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
  glEnableVertexAttribArray(m_NumLayouts + 3);
  glVertexAttribPointer(m_NumLayouts + 3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

  glVertexAttribDivisor(m_NumLayouts, 1);
  glVertexAttribDivisor(m_NumLayouts + 1, 1);
  glVertexAttribDivisor(m_NumLayouts + 2, 1);
  glVertexAttribDivisor(m_NumLayouts + 3, 1);

  glBindVertexArray(0);
}

template <typename VertexType>
void Mesh<VertexType>::Draw(std::shared_ptr<Shader> shader, DrawMethod dMethod) {
  glBindVertexArray(VAO);

  if (!material.Empty()) {
    material.Bind(*shader);
  }
  glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
  switch (dMethod) {
    case DrawMethod::TRIANGLE_SINGLE:
      shader->setUniformBool("u_instanceDraw", false);
      glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
      break;
    case DrawMethod::TRIANGLE_INSTANCED:
      if (!m_instance_mat->empty()) {
        shader->setUniformBool("u_instanceDraw", true);
        glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0,
                                m_instance_mat->size());
      }
      break;
  }
  unbindTextures();
  glBindVertexArray(0);
}

template <typename VertexType>
void Mesh<VertexType>::setMaterial(const Material& mat) {
  material = mat;
}

std::array<VertexAttribute, 4> Vertex::layout() {
  return {{{0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, position)},
           {1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, Normal)},
           {2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, TexCoord)},
           {3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, Tangent)}}};
}

std::array<VertexAttribute, 1> VertexP::layout() {
  return {{0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexP), offsetof(VertexP, position)}};
}

std::array<VertexAttribute, 2> VertexPN::layout() {
  return {{{0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, position)},
           {1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, Normal)}}};
}

std::array<VertexAttribute, 3> VertexPNT::layout() {
  return {{{0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, position)},
           {1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, Normal)},
           {2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, TexCoord)}}};
}
