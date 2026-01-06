//PHONG_GLSL

struct PhongMaterial{
  vec4 diffuse;
  vec4 specular;
  vec4 ambient;
  float shininess;
  float shininessStrength;

  bool hasDiffuseMap;
  bool hasSpecularMap;
  bool hasAmbientMap;
  bool hasNormalMap;
};

uniform sampler2D u_diffuseMap;
uniform sampler2D u_specularMap;
uniform sampler2D u_ambientMap;
uniform sampler2D u_normalMap;

uniform PhongMaterial materialPhong;

vec4 CalculatePhong(vec2 TexCoord,vec3 FragPos,vec3 Normal,vec3 TangentFragPos, mat3 TBN);

vec4 CalculatePhong(vec2 TexCoord,vec3 FragPos,vec3 Normal,vec3 TangentFragPos, mat3 TBN){
    vec4 DiffColor = materialPhong.diffuse;
    vec4 SpecColor = materialPhong.specular;
    vec4 AmbColor = materialPhong.diffuse;

    vec3 norm = normalize(Normal);
    vec3 lightPos = vec3(0,1,0);
    vec3 mFragPos = FragPos;
    vec3 mViewPos = viewPos;

    if(materialPhong.hasDiffuseMap){
      DiffColor = texture(u_diffuseMap, TexCoord);
    }
    if(materialPhong.hasSpecularMap){
      SpecColor = texture(u_specularMap, TexCoord);
    }
    if(materialPhong.hasAmbientMap){
      AmbColor = texture(u_ambientMap, TexCoord);
    }
    if (materialPhong.hasNormalMap) {
        lightPos = TBN * lightPos;
        mViewPos  = TBN * viewPos;
        mFragPos = TangentFragPos;

        norm = texture(u_normalMap, TexCoord).rgb;
        norm = normalize(norm * 2.0 - 1.0);
        norm = normalize(TBN * norm);
    }

    vec3 lightColor = vec3(1.0,1.0,1.0);
    vec3 lightAmb = lightColor * 0.1;
    vec3 lightSpec = lightColor * 1.1;
    float lightStr = 0.5;
    lightColor *= lightStr;
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightAmb * AmbColor.rgb;
  	
    // diffuse 
    vec3 lightDir = normalize(vec3(-1,-0.2,-1));
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * DiffColor.rgb;
    
    // specular
    vec3 viewDir = normalize(mViewPos - mFragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
    vec3 halfWay = normalize(viewDir + lightDir);
    float spec = pow(max(dot(viewDir, halfWay), 0.0),materialPhong.shininess);
    vec3 specular = spec * lightSpec * SpecColor.rgb;  

    vec3 result = ambient + diffuse + specular;
    return vec4(result,1);
}
