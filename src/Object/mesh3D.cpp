#include "mesh3D.h"

#include <array>
#include <iostream>

#include "glad/glad.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Material material, std::string& name,
           Type type) {
  m_name = name;
  m_type = type;
  this->vertices = vertices;
  this->indices = indices;
  this->material = material;
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::string& name, Type type) {
  m_name = name;
  m_type = type;
  this->vertices = vertices;
  this->indices = indices;
}

/*Mesh::~Mesh(){
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&EBO);
    glDeleteVertexArrays(1,&VAO);
}*/

void Mesh::CreateVAO() {
  if (glIsVertexArray(VAO)) {
    std::cerr << m_name << "--> VAO" << VAO << " already exists!!\n";
    return;
  }
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  std::vector<VertexAttribute> attributes;
  switch (m_type) {
    case Type::VERTEXP:
      m_createVBO<VertexP>();
      attributes.assign(VertexP::layout().begin(), VertexP::layout().end());
      break;
    case Type::VERTEXPN:
      m_createVBO<VertexPN>();
      attributes.assign(VertexPN::layout().begin(), VertexPN::layout().end());
      break;
    case Type::VERTEXPNT:
      m_createVBO<VertexPNT>();
      attributes.assign(VertexPNT::layout().begin(), VertexPNT::layout().end());
      break;
    case Type::VERTEX:
      glGenBuffers(1, &VBO);
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
      attributes.assign(Vertex::layout().begin(), Vertex::layout().end());
      break;
  }
  if (!indices.empty()) {
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    size_t sizeEBO = sizeof(glm::vec3);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
  }

  for (const auto& attr : attributes) {
    glEnableVertexAttribArray(attr.index);
    glVertexAttribPointer(attr.index, attr.size, attr.type, attr.normalized, attr.stride, (void*)(attr.offset));
  }

  std::cout << attributes.size() << std::endl;
  glBindVertexArray(0);
}

void Mesh::makeInstanced(std::shared_ptr<std::vector<glm::mat4>> tranformMatArr) {
  if (tranformMatArr->empty()) {
    return;
  }
  m_instance_mat = tranformMatArr;
  glBindVertexArray(VAO);
  glGenBuffers(1, &instanceVBO);
  glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
  glBufferData(GL_ARRAY_BUFFER, m_instance_mat->size() * sizeof(glm::mat4), m_instance_mat->data(), GL_STATIC_DRAW);

  std::size_t vec4Size = sizeof(glm::vec4);
  glEnableVertexAttribArray(INSTANCE_ATTRIB_I);
  glVertexAttribPointer(INSTANCE_ATTRIB_I, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
  glEnableVertexAttribArray(INSTANCE_ATTRIB_I + 1);
  glVertexAttribPointer(INSTANCE_ATTRIB_I + 1, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
  glEnableVertexAttribArray(INSTANCE_ATTRIB_I + 2);
  glVertexAttribPointer(INSTANCE_ATTRIB_I + 2, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
  glEnableVertexAttribArray(INSTANCE_ATTRIB_I + 3);
  glVertexAttribPointer(INSTANCE_ATTRIB_I + 3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

  glVertexAttribDivisor(INSTANCE_ATTRIB_I, 1);
  glVertexAttribDivisor(INSTANCE_ATTRIB_I + 1, 1);
  glVertexAttribDivisor(INSTANCE_ATTRIB_I + 2, 1);
  glVertexAttribDivisor(INSTANCE_ATTRIB_I + 3, 1);

  glBindVertexArray(0);
}

void Mesh::Draw(std::shared_ptr<Shader> shader, DrawMethod dMethod) {
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

void Mesh::setMaterial(const Material& mat) { material = mat; }

VertexP::VertexP(const Vertex& v) { position = v.position; }
VertexPN::VertexPN(const Vertex& v) {
  position = v.position;
  Normal = v.Normal;
}
VertexPNT::VertexPNT(const Vertex& v) {
  position = v.position;
  Normal = v.Normal;
  TexCoord = v.TexCoord;
}

std::array<VertexAttribute, 6> Vertex::layout() {
  return {{{POS_ATTRIB_I, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, position)},
           {NORM_ATTRIB_I, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, Normal)},
           {UI_ATTRIB_I, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, TexCoord)},
           {TANGENT_ATTRIB_I, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, Tangent)},
           {BONE_ATTRIB_I, 4, GL_INT, sizeof(Vertex), offsetof(Vertex, m_BoneIDs)},
           {WEIGHT_ATTRIB_I, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, m_Weights)}}};
}

std::array<VertexAttribute, 1> VertexP::layout() {
  return {{0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexP), offsetof(VertexP, position)}};
}

std::array<VertexAttribute, 2> VertexPN::layout() {
  return {{{0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), offsetof(VertexPN, position)},
           {1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), offsetof(VertexPN, Normal)}}};
}

std::array<VertexAttribute, 3> VertexPNT::layout() {
  return {{{0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, position)},
           {1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, Normal)},
           {2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, TexCoord)}}};
}

std::array<VertexAttribute, 4> VertexPNTT::layout() {
  return {{{POS_ATTRIB_I, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, position)},
           {NORM_ATTRIB_I, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, Normal)},
           {UI_ATTRIB_I, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, TexCoord)},
           {TANGENT_ATTRIB_I, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, Tangent)}}};
}
