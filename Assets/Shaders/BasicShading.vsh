#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 gMatrixWVP;
uniform mat4 gMatrixViewInverse;
uniform mat4 gMatrixWorld;

void main()
{
    gl_Position = gMatrixWVP * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}