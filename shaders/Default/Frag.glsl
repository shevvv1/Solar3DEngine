#version 420 core

#include "Phong.glsl"

out vec4 FragColor;

in VS_OUT{
  vec2 TexCoord;
  vec3 FragPos;
  vec3 Normal;
  mat3 TBN;
}fs_in;


void main()
{
 FragColor = CalculatePhong(fs_in.TexCoord,fs_in.FragPos,fs_in.Normal,fs_in.TBN);
   // FragColor = vec4(1,0,0,1);
}

