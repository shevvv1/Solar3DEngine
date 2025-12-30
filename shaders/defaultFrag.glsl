#version 330 core

out vec4 FragColor;

in VS_OUT{
  vec2 TexCoord;
  vec3 FragPos;
  vec3 Normal;
  vec3 TangentFragPos;
  mat3 TBN;
}fs_in;

#include "phong.glsl"

void main()
{
  FragColor = CalculatePhong(fs_in.TexCoord,fs_in.FragPos,fs_in.Normal,fs_in.TangentFragPos,fs_in.TBN);
}
