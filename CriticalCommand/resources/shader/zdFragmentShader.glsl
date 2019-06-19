#version 330 core

layout(location = 0) out vec4 color;

//in mat4 model;
//in mat4 view;
//in mat4 projection;

in vec3 position;

void main(){
if (position.x > 1)
	color =  vec4(0.4, 0.0, 0.5, 1.0);
else
	color =  vec4(0.0, 0.0, 0.5, 1.0);
};