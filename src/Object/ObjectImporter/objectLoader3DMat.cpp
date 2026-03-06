#include <assimp/color4.h>
#include <assimp/material.h>

#include <vector>

#include "CacheManager/cacheManager.h"
#include "File/utils.h"
#include "Object/Material/material.h"
#include "objectLoader3D.h"

namespace Object3DImport {

struct AiMatKey {
  const char* key;
  unsigned int type;
  unsigned int index;

  AiMatKey(const char* k = nullptr, unsigned t = 0, unsigned i = 0) : key(k), type(t), index(i) {}

  bool operator==(const AiMatKey& other) const {
    return key == other.key && type == other.type && index == other.index;
  }
};
struct AiTextureMap {
  const char* u_name;
  aiTextureType aiType;
  int flagBit;
};

struct AiMatProp {
  std::string u_name;
  AiMatKey key;
  int flagBit;
};

static const std::vector<AiTextureMap> AiMatTextureList = {
    {"u_albedoMap", aiTextureType_DIFFUSE, MaterialFlags::HAS_ALBEDO_MAP},
    {"u_albedoMap", aiTextureType_BASE_COLOR, MaterialFlags::HAS_ALBEDO_MAP},
    {"u_specularMap", aiTextureType_SPECULAR, MaterialFlags::HAS_SPECULAR_MAP},
    {"u_aoMap", aiTextureType_AMBIENT, MaterialFlags::HAS_AO_MAP},
    {"u_aoMap", aiTextureType_AMBIENT_OCCLUSION, MaterialFlags::HAS_AO_MAP},
    {"u_normalMap", aiTextureType_NORMALS, MaterialFlags::HAS_NORMAL_MAP},
    {"u_metallicMap", aiTextureType_METALNESS, MaterialFlags::HAS_METALLIC_MAP},
    {"u_roughnessMap", aiTextureType_DIFFUSE_ROUGHNESS, MaterialFlags::HAS_ROUGHNESS_MAP},
    {"u_metallicRoughnessMap", aiTextureType_GLTF_METALLIC_ROUGHNESS, MaterialFlags::HAS_METALIC_ROUGHNESS_MAP}};

static const std::vector<AiMatProp> AiMatColorList = {
    {"u_albedoColor", {AI_MATKEY_COLOR_DIFFUSE}, MaterialFlags::HAS_ALBEDO_COLOR},
    {"u_specularColor", {AI_MATKEY_COLOR_SPECULAR}, MaterialFlags::HAS_SPECULAR_COLOR},
    {"u_ambientColor", {AI_MATKEY_COLOR_AMBIENT}, MaterialFlags::HAS_AO_COLOR}};

static const std::vector<AiMatProp> MatPropList = {
    {"u_metallicFactor", {AI_MATKEY_METALLIC_FACTOR}, 0},
    {"u_roughnessFactor", {AI_MATKEY_ROUGHNESS_FACTOR}, 0},
    {"u_shininess", {AI_MATKEY_SHININESS}, 0},
};

Material AssimpLoader::m_loadMaterial(aiMaterial* mat) {
  Material material;
  material.setName(mat->GetName().data);

  MaterialType mat_type = m_detectMaterialType(mat);

  material.setType(mat_type);

  Material::Data MatData;

  aiString str;
  std::string tex_path;
  for (auto& it : AiMatTextureList) {
    if (mat->GetTexture(it.aiType, 0, &str) == AI_SUCCESS) {
      tex_path = m_rootDir + '/' + str.C_Str();
      tex_path = FileUtils::convertTextureFilePath(str.C_Str(), m_rootDir.c_str());
      material.setNewMap(it.u_name, TextureManager::getTexture(tex_path));
      MatData.flags |= it.flagBit;
      str.Clear();
    }
  }

  for (auto& it : AiMatColorList) {
    aiColor4D aiCol;
    if (mat->Get(it.key.key, it.key.type, it.key.index, aiCol) == AI_SUCCESS) {
      switch (it.flagBit) {
        case MaterialFlags::HAS_ALBEDO_COLOR:
          MatData.albedo = glm::vec4(aiCol.r, aiCol.g, aiCol.b, aiCol.a);
          MatData.flags |= it.flagBit;
          break;
        case MaterialFlags::HAS_SPECULAR_COLOR:
          MatData.specular = glm::vec4(aiCol.r, aiCol.g, aiCol.b, aiCol.a);
          MatData.flags |= it.flagBit;
          break;
        case MaterialFlags::HAS_AO_COLOR:
          MatData.ambient = glm::vec4(aiCol.r, aiCol.g, aiCol.b, aiCol.a);
          MatData.flags |= it.flagBit;
          break;
        default:
          material.setNewColor(it.u_name, glm::vec4(aiCol.r, aiCol.g, aiCol.b, aiCol.a));
      }
    }
  }

  for (auto& it : MatPropList) {
    float aiProp;
    if (mat->Get(it.key.key, it.key.type, it.key.index, aiProp) == AI_SUCCESS) {
      if (it.u_name == "u_metallicFactor") {
        MatData.metallness = aiProp;
      } else if (it.u_name == "u_roughnessFactor") {
        MatData.roghness = aiProp;
      } else {
        material.setNewFloat(it.u_name, aiProp);
      }
    }
  }

  material.setUBOdata(MatData);

  return material;
}

MaterialType AssimpLoader::m_detectMaterialType(aiMaterial* material) {
  bool hasPBRMaps = (material->GetTextureCount(aiTextureType_METALNESS) > 0) ||
                    (material->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS) > 0) ||
                    (material->GetTextureCount(aiTextureType_GLTF_METALLIC_ROUGHNESS) > 0);
  float f;
  bool hasPBRprop = (material->Get(AI_MATKEY_METALLIC_FACTOR, f) == AI_SUCCESS ||
                     material->Get(AI_MATKEY_ROUGHNESS_FACTOR, f) == AI_SUCCESS);
  if (hasPBRMaps || hasPBRprop) {
    return MaterialType::PBR;
  } else {
    return MaterialType::PHONG;
  }
}
}  // namespace Object3DImport
