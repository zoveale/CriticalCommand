//Environment Map
#version 460 core
layout (location = 0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 view;

out vec3 WorldPos;

void main()
{
    WorldPos = aPos;

	mat4 rotView = view;
	vec4 clipPos = projection * rotView * vec4(WorldPos, 1.0);

	//xyww trick here that ensures the depth value of the 
	//rendered cube fragments always end up at 1.0
	gl_Position = clipPos.xyww;
}