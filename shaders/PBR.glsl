//PBR_GLSL

struct PBRMaterial{
  vec4 diffuse;
  vec4 specular;
  vec4 ambient;
  float shininess;
  float shininessStrength;

  bool hasBaseColorMap;
  bool hasMetallicMap;
  bool hasRoughnessMap;
  bool hasNormalMap;
};

uniform sampler2D u_BaseMap;
uniform sampler2D u_MetallicMap;
uniform sampler2D u_RoughnessMap;
uniform sampler2D u_normalMap;



