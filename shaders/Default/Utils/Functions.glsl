// Utils/Functions
#include "Uniforms/Uni.glsl"
#include "Uniforms/MaterialUni.glsl"

vec4 GetAlbedoFromUni(vec2 TexCoord);
vec4 GetSpecFromUni(vec2 TexCoord);
vec4 GetAOFromUni(vec2 TexCoord);
vec3 GetNormalFromUni(mat3 TBN,vec3 Normal,vec2 TexCoord);

float GetMetallicFromUni(vec2 TexCoord);
float GetRoughnessFromUni(vec2 TexCoord);

vec3 notCompleteZero(vec3 V);

vec4 GetAlbedoFromUni(vec2 TexCoord){
  vec4 AlbedoColor = vec4(0.5,0.5,0.5,1.0);
  if(u_albedoMap_flag){
    AlbedoColor = texture(u_albedoMap,TexCoord);
    return vec4(notCompleteZero(AlbedoColor.rgb),AlbedoColor.a);
  }
  if(u_albedoColor_flag){
    AlbedoColor = u_albedoColor;
    return vec4(notCompleteZero(AlbedoColor.rgb),AlbedoColor.a);
  }
 
  return AlbedoColor;
}

vec4 GetSpecFromUni(vec2 TexCoord){
  vec4 SpecColor = vec4(0.8,0.8,0.8,1.0);
  if(u_specularMap_flag){
    SpecColor = texture(u_specularMap,TexCoord);
    return vec4(notCompleteZero(SpecColor.rgb),SpecColor.a);
  }
  if(u_ambientColor_flag){
    SpecColor = u_specularColor;
    return vec4(notCompleteZero(SpecColor.rgb),SpecColor.a);
  }

  if(g_hasAlbedo){
    SpecColor = GetAlbedoFromUni(TexCoord);
  }
  return SpecColor;
}

vec4 GetAOFromUni(vec2 TexCoord){
  vec4 AOColor = vec4(0.1,0.1,0.1,1.0);
  if(u_aoMap_flag){
    AOColor = texture(u_aoMap,TexCoord);
    return vec4(notCompleteZero(AOColor.rgb),AOColor.a);
  }
  if(u_ambientColor_flag){
    AOColor = u_ambientColor;
    return vec4(notCompleteZero(AOColor.rgb),AOColor.a);
  }
  if(g_hasAlbedo){
    AOColor = GetAlbedoFromUni(TexCoord) * 0.3;
  }
   return AOColor;
}

vec3 GetNormalFromUni(mat3 TBN, vec3 Normal, vec2 TexCoord){
  if (u_normalMap_flag) {
    vec3 tangentNormal = texture(u_normalMap, TexCoord).rgb * 2.0 - 1.0;
    return normalize(TBN * tangentNormal);
  } 
  return Normal;
}

float GetMetallicFromUni(vec2 TexCoord){
  if(u_metallicMap_flag){
   return texture(u_metallicMap, TexCoord).r;
  }
  if(u_metallicRoughnessMap_flag){
    return texture(u_metallicRoughnessMap, TexCoord).b;
  }
  return u_metallicFactor;
}

float GetRoughnessFromUni(vec2 TexCoord){
  if(u_roughnessMap_flag){
    return texture(u_roughnessMap, TexCoord).r;
  }
  if(u_metallicRoughnessMap_flag){
    return texture(u_metallicRoughnessMap, TexCoord).g;
  }
  return u_roughnessFactor;
}

vec3 notCompleteZero(vec3 V){
  if(V.r == 0.0 && V.g == 0.0 && V.b == 0.0){
    return V.rgb + vec3(0.01);
  }
  return V;
}
