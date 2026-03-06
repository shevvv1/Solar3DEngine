//PHONG_GLSL
#include "Uniforms/Uni.glsl"
#include "Uniforms/MaterialUni.glsl"
#include "Utils/Functions.glsl"

vec4 CalculatePhong(vec2 TexCoord,vec3 FragPos,vec3 Normal, mat3 TBN);

vec4 CalculatePhong(vec2 TexCoord,vec3 FragPos,vec3 Normal, mat3 TBN){
    vec4 DiffColor =  GetAlbedoFromUni(TexCoord);
    vec4 SpecColor = DiffColor;
    vec4 AmbColor = DiffColor;

    vec3 lightPos = vec3(0,100,100);

    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 lightColor = vec3(1.0,1.0,1.0);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfWay = normalize(viewDir + lightDir);

    vec3 normal = GetNormalFromUni(TBN, Normal, TexCoord);
    float diff = max(dot(normal, lightDir), 0.0) * 0.5 + 0.5;
    diff *= diff;
    float spec = pow(max(dot(normal, halfWay), 0.0),8);
    spec *= (8 + 8.0) / (8.0 * 3.14159265);
 
    float ambientStrength = 0.0;
    vec3 ambient = AmbColor.rgb * ambientStrength;

    vec3 diffuse = diff * DiffColor.rgb;

    vec3 specular = spec * SpecColor.rgb * lightColor;  

    vec3 result = diffuse + specular + ambient;
    return vec4(result,1.0);
}
