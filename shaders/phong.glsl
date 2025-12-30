#version 330 core
out vec4 FragColor;


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

in vec3 FragPos;  
in vec2 TexCoord;
in vec3 Normal;  
in vec3 TangentFragPos;
in mat3 TBN;

uniform sampler2D u_diffuseMap;
uniform sampler2D u_specularMap;
uniform sampler2D u_ambientMap;
uniform sampler2D u_normalMap;

uniform vec3 viewPos; 
uniform PhongMaterial materialPhong;

void main()
{
     
    vec4 DiffColor = materialPhong.diffuse;
    vec4 SpecColor = materialPhong.specular;
    vec4 AmbColor = materialPhong.diffuse;

    if(materialPhong.hasDiffuseMap){
      DiffColor = texture(u_diffuseMap, TexCoord);
    }
    if(materialPhong.hasSpecularMap){
      SpecColor = texture(u_specularMap, TexCoord);
    }
    if(materialPhong.hasAmbientMap){
      AmbColor = texture(u_ambientMap, TexCoord);
    }
    vec3 norm = normalize(Normal);
    vec3 lightPos = vec3(0,0,0);
    vec3 mFragPos = FragPos;
    vec3 mViewPos = viewPos;
    if (materialPhong.hasNormalMap) {
        lightPos = TBN * lightPos;
        mViewPos  = TBN * viewPos;
        mFragPos = TangentFragPos;

        norm = texture(u_normalMap, TexCoord).rgb;
        norm = norm * 2.0 - 1.0;
        norm = -normalize(TBN * norm);
    }

    vec3 lightColor = vec3(1.0,1.0,1.0);
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor * AmbColor.rgb;
  	
    // diffuse 
    vec3 lightDir = normalize(lightPos - mFragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * DiffColor.rgb;
    
    // specular
    vec3 viewDir = normalize(mViewPos - mFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0),materialPhong.shininess);
    vec3 specular = spec * lightColor * SpecColor.rgb;  

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result,1);
}
