#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 texCoords;

uniform mat4 u_PM;
uniform mat4 u_VM;

void main()
{
    texCoords = vec3(aPos.x, aPos.y, -aPos.z);
    vec4 pos = u_PM * u_VM * vec4(aPos, 1.0f);
    // Having z equal w will always result in a depth of 1.0f
    gl_Position = vec4(pos.x,pos.y, pos.w, pos.w);
    // We want to flip the z axis due to the different coordinate systems (left hand vs right hand)
}
