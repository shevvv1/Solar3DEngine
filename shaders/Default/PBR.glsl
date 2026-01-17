//PBR_GLSL
#include "Uniforms/Uni.glsl"
#include "Uniforms/MaterialUni.glsl"
#include "Utils/Functions.glsl"

const float PI = 3.14159265359;

vec4 CalculatePBR(vec2 TexCoord,vec3 FragPos,vec3 Normal,vec3 TangentFragPos, mat3 TBN);
// GGX/Trowbridge-Reitz Normal Distribution Function
float D(float alpha,vec3 N,vec3 H);
// Smith Model
float G(vec3 N, vec3 V, vec3 L, float roughness);
// fresnelSchlick function
vec3 F(vec3 F0,vec3 V,vec3 H);

vec4 CalculatePBR(vec2 TexCoord,vec3 FragPos,vec3 Normal,vec3 TangentFragPos, mat3 TBN){
  vec4 AlbedoColor = GetAlbedoFromUni(TexCoord);
  vec4 AmbColor = GetAOFromUni(TexCoord);

  vec3 normal = GetNormalFromUni(TBN,Normal,TexCoord);
  float metallic = GetMetallicFromUni(TexCoord);
  float roughness = GetRoughnessFromUni(TexCoord);

  vec3 lightColor = vec3(0.8,0.8,0.8);
  vec3 F0 = vec3(0.04);
  F0 = mix(F0,AlbedoColor.rgb,metallic);
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 lightPos = vec3(2.5,3,1);
  vec3 lightDir = normalize(-vec3(1, -1.0, -1.0));
  vec3 halfWay = normalize(viewDir + lightDir);
   float distance    = length(lightPos - FragPos);
        float attenuation = 1.0 / (distance);
        vec3 radiance     = lightColor * 5; 
 
  float NDF = D(roughness,normal,halfWay);
  float G = G(normal,viewDir,lightDir,roughness);
  vec3 F = F(F0,viewDir,halfWay);

  vec3 kS = F;
  vec3 kD = vec3(1.0) - kS;
  kD *= 1.0 - metallic;
  
  vec3 numerator    = NDF * G * F;
  float denominator = 4.0 * max(dot(normal, viewDir), 0.0) * max(dot(normal, lightDir), 0.0) + 0.0001;
  vec3 specular     = numerator / denominator;  
  //
        // add to outgoing radiance Lo
  float NdotL = max(dot(normal, lightDir), 0.0);                
  vec3 Lo = (kD * AlbedoColor.rgb / PI + specular) * radiance * NdotL;

  vec3 ambient = vec3(0.3) * AlbedoColor.rgb * AmbColor.rgb;
  vec3 color = ambient + Lo;
	
  color = color / (color + vec3(1.0));
   
  return vec4(color,AlbedoColor.a);
}


float D(float roughness,vec3 N,vec3 H){
  float numerator = pow(roughness, 4.0);
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
    
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
    
    return a2 / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}

float G(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

vec3 F(vec3 F0,vec3 V,vec3 H)
{
    return F0 + (vec3(1.0) - F0) * pow(1.0 - max(dot(V,H), 0.0), 5.0);
}  
