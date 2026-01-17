#version 330 core

#include "PBR.glsl"
#include "Phong.glsl"


out vec4 FragColor;

in VS_OUT{
  vec2 TexCoord;
  vec3 FragPos;
  vec3 Normal;
  vec3 TangentFragPos;
  mat3 TBN;
}fs_in;


void main()
{
  if(u_MatType == MAT_TYPE_PHONG){
    FragColor = CalculatePhong(fs_in.TexCoord,fs_in.FragPos,fs_in.Normal,fs_in.TangentFragPos,fs_in.TBN);
  }
  if(u_MatType == MAT_TYPE_PBR){
    FragColor = CalculatePBR(fs_in.TexCoord,fs_in.FragPos,fs_in.Normal,fs_in.TangentFragPos,fs_in.TBN);
  }
}

