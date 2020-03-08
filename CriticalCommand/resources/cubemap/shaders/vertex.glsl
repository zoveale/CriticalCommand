//skybox/cubemap
#version 460 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
    vec4 pos  = projection * view * vec4(aPos, 1.0);

	//xyww trick here that ensures the depth value of the 
	//rendered cube fragments always end up at 1.0
	gl_Position = pos.xyww;
} 