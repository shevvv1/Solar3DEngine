//PHONG_GLSL
#include "Uniforms/Uni.glsl"
#include "Uniforms/MaterialUni.glsl"

vec4 CalculatePhong(vec2 TexCoord,vec3 FragPos,vec3 Normal,vec3 TangentFragPos, mat3 TBN);

vec4 CalculatePhong(vec2 TexCoord,vec3 FragPos,vec3 Normal,vec3 TangentFragPos, mat3 TBN){
    vec4 DiffColor =  GetAlbedoFromUni(TexCoord);
    vec4 SpecColor = GetSpecFromUni(TexCoord);
    vec4 AmbColor = GetAOFromUni(TexCoord);

    vec3 lightPos = vec3(2.5,10,1);

    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 lightColor = vec3(1.0,1.0,1.0);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfWay = normalize(viewDir + lightDir);

    vec3 normal = GetNormalFromUni(TBN, Normal, TexCoord);
    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(normal, halfWay), 0.0),8);
 
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = AmbColor.rgb * ambientStrength;
    // diffuse 
    vec3 diffuse = diff * DiffColor.rgb;
    
    // specular
    vec3 specular = spec * SpecColor.rgb * lightColor;  

    vec3 result = diffuse + specular + ambient;
    result = pow(result, vec3(1.0/1.2));
    return vec4(result,DiffColor.a);
}
