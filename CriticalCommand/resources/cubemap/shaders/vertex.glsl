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

	//have the z position always equal to zero for
	//depth testing purposes
	gl_Position = pos.xyww;
} 