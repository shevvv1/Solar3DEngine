// Utils/Functions
#include "Uniforms/Uni.glsl"
#include "Uniforms/MaterialUni.glsl"

vec4 GetAlbedoFromUni(vec2 TexCoord);
vec3 GetNormalFromUni(mat3 TBN,vec3 Normal,vec2 TexCoord);

vec4 GetAlbedoFromUni(vec2 TexCoord){
  vec4 AlbedoColor = vec4(0.5,0.5,0.5,1.0);
  if((u_matFlags & HAS_ALBEDO_MAP)!=0){
    AlbedoColor = texture(u_albedoMap,TexCoord);
    return AlbedoColor;
  }
  if((u_matFlags & HAS_ALBEDO_COLOR)!=0){
    AlbedoColor = u_albedoColor;
    return AlbedoColor;
  }
 
  return AlbedoColor;
}

vec3 GetNormalFromUni(mat3 TBN, vec3 Normal, vec2 TexCoord){
  if ((u_matFlags & HAS_NORMAL_MAP)!=0) {
    vec3 tangentNormal = texture(u_normalMap, TexCoord).rgb * 2.0 - 1.0;
    return normalize(TBN * tangentNormal);
  } 
  return Normal;
}
