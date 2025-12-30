#version 420 compatibility
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

uniform mat4 u_PM;
uniform mat4 u_VM;
uniform mat4 u_MM; 

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;
out vec3 TangentFragPos;
out mat3 TBN;

void main()
{
	FragPos = vec3(u_MM * vec4(aPos, 1.0));
  vec3 T = normalize(vec3(u_MM * vec4(aTangent,0.0)));
  vec3 N = normalize(vec3(u_MM * vec4(aNormal,0.0)));
  T = normalize(T - dot(T, N) * N);
  vec3 B = cross(N, T);
  TBN = transpose(mat3(T, B, N));

  TangentFragPos  = TBN * vec3(u_MM * vec4(aPos, 1.0));

  Normal = mat3(transpose(inverse(u_MM))) * aNormal;
	TexCoord = aTexCoord;
  gl_Position = u_PM * u_VM * u_MM * vec4(aPos, 1.0);
}
