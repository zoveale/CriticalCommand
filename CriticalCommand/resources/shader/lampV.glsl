
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitTangent;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 PVM;

void main()
{
	gl_Position = PVM * vec4(position, 1.0);
}