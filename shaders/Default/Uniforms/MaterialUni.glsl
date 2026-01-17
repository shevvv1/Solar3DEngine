// defaultMaterialUniforms
//
#define MAT_TYPE_PHONG 0
#define MAT_TYPE_PBR 1

uniform int u_MatType;

//Map textures
uniform sampler2D u_albedoMap;
uniform sampler2D u_specularMap;
uniform sampler2D u_aoMap;
uniform sampler2D u_normalMap;
uniform sampler2D u_heightMap;
uniform sampler2D u_emissiveMap;
uniform sampler2D u_metallicMap;
uniform sampler2D u_roughnessMap;
uniform sampler2D u_metallicRoughnessMap;
uniform sampler2D u_lightMap;

//Map_flags
uniform bool u_albedoMap_flag;
uniform bool u_specularMap_flag;
uniform bool u_aoMap_flag;
uniform bool u_normalMap_flag;
uniform bool u_heightMap_flag;
uniform bool u_emissiveMap_flag;
uniform bool u_metallicMap_flag;
uniform bool u_roughnessMap_flag;
uniform bool u_metallicRoughnessMap_flag;
uniform bool u_lightMap_flag;

//Colors
uniform vec4 u_albedoColor;
uniform vec4 u_specularColor;
uniform vec4 u_ambientColor;
uniform vec4 u_emissiveColor;
uniform vec4 u_transparentColor;
uniform vec4 u_reflectiveColor;

//Colors_flags
uniform bool u_albedoColor_flag;
uniform bool u_specularColor_flag;
uniform bool u_ambientColor_flag;
uniform bool u_emissiveColor_flag;
uniform bool u_transparentColor_flag;
uniform bool u_reflectiveColor_flag;


//Properties
uniform float u_metallicFactor;
uniform float u_roughnessFactor;
uniform float u_shininess;
uniform float u_shininessStrength;
uniform float u_reflectivity;
uniform float u_refractionIndex;
uniform float u_opacity;
uniform float u_transparencyFactor;
uniform float u_bumpScaling;


bool g_hasAlbedo = u_albedoMap_flag && u_albedoColor_flag;
