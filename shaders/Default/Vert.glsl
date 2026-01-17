#version 330 compatibility
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

uniform mat4 u_PM;
uniform mat4 u_VM;
uniform mat4 u_MM; 
uniform mat3 u_NM;

uniform mat3 TBN;

out VS_OUT{
vec2 TexCoord;
vec3 FragPos;
vec3 Normal;
vec3 TangentFragPos;
mat3 TBN;
}vs_out;

void main()
{
	vs_out.FragPos = vec3(u_MM * vec4(aPos, 1.0));
  vec3 T = normalize(vec3(u_MM * vec4(aTangent,0.0)));
  vec3 N = normalize(vec3(u_MM * vec4(aNormal,0.0)));
  vec3 B = cross(T,N);
  //B = aBitangent;
  T = normalize(T - dot(T, N) * N);
  vs_out.TBN = mat3(T, B, N);

  vs_out.TangentFragPos  = vs_out.TBN * vec3(u_MM * vec4(aPos, 1.0));

  vs_out.Normal = u_NM * aNormal;
	vs_out.TexCoord = aTexCoord;
  gl_Position = u_PM * u_VM * u_MM * vec4(aPos, 1.0);
}
