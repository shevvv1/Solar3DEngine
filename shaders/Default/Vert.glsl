#version 420 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in ivec4 aBoneIds;
layout (location = 5) in vec4 aWeights;
layout (location = 6) in mat4 aInstanceMatrix;

const int MAX_BONES = 200;
const int MAX_BONE_INFLUENCE = 4;

uniform mat4 u_Bones[MAX_BONES];
uniform mat4 u_PM;
uniform mat4 u_VM;
uniform mat4 u_MM; 
uniform mat3 u_NM;
uniform bool u_instanceDraw;
uniform bool hasBone;

out VS_OUT {
    vec2 TexCoord;
    vec3 FragPos;
    vec3 Normal;
    mat3 TBN; 
} vs_out;

void main() {
    vec4 totalPosition = vec4(aPos, 1.0);
    vec3 totalNormal = aNormal;
    
    if (hasBone) {
        totalPosition = vec4(0.0);
        totalNormal = vec3(0.0);

        for (int i = 0; i < MAX_BONE_INFLUENCE; i++) {
            if (aBoneIds[i] == -1) continue;
            if (aBoneIds[i] >= MAX_BONES) {
                totalPosition = vec4(aPos, 1.0);
                break;
            }
            mat4 boneTransform = u_Bones[aBoneIds[i]];
            totalPosition += boneTransform * vec4(aPos, 1.0) * aWeights[i];
            totalNormal += mat3(boneTransform) * aNormal * aWeights[i];
        }
    }
    
    mat4 worldMatrix = u_MM;
    if (u_instanceDraw) {
        worldMatrix = aInstanceMatrix * u_MM;
    }
    
    vs_out.FragPos = vec3(worldMatrix * totalPosition);
    
    vec3 T = normalize(vec3(worldMatrix * vec4(aTangent, 0.0)));
    vec3 N = normalize(vec3(worldMatrix * vec4(totalNormal, 0.0)));
    vec3 B = cross(T, N);
    T = normalize(T - dot(T, N) * N);
    vs_out.TBN = mat3(T, B, N);
    
    vs_out.Normal = normalize(mat3(worldMatrix) * totalNormal);
    
    vs_out.TexCoord = aTexCoord;
    
    gl_Position = u_PM * u_VM * worldMatrix * totalPosition;
}
