// defaultMaterialUniforms
//
#define MAT_TYPE_PHONG 0
#define MAT_TYPE_PBR 1

uniform int u_MatType;

layout(std140,binding = 1) uniform MatData{
    int u_matFlags;
    float shininess;
    float metallness;
    float roghness;
    vec4 u_albedoColor;
    vec4 specular;
    vec4 ambient;
}; 

//Map textures
uniform sampler2D u_albedoMap;
uniform sampler2D u_specularMap;
uniform sampler2D u_aoMap;
uniform sampler2D u_normalMap;
uniform sampler2D u_metallicMap;
uniform sampler2D u_roughnessMap;
uniform sampler2D u_metallicRoughnessMap;

//Properties
uniform float u_metallicFactor;
uniform float u_roughnessFactor;

//flags
 const int HAS_ALBEDO_MAP = 1 << 0;
 const int  HAS_NORMAL_MAP = 1 << 1;
 const int HAS_METALLIC_MAP = 1 << 2;
 const int HAS_ROUGHNESS_MAP = 1 << 3;
 const int HAS_METALIC_ROUGHNESS_MAP = 1 << 4;
 const int HAS_AO_MAP = 1 << 5;
 const int HAS_EMISSIVE_MAP = 1 << 6;
 const int HAS_SPECULAR_MAP = 1 << 7;
 const int HAS_HEIGHT_MAP = 1 << 8;

 const int HAS_ALBEDO_COLOR = 1 << 9;
 const int HAS_SPECULAR_COLOR = 1 << 10;
 const int HAS_AO_COLOR = 1 << 11;
 const int HAS_EMISSIVE_COLOR = 1 << 12;
