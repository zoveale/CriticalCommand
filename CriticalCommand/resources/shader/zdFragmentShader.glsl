#version 330 core

out vec4 color;

//in mat4 model;
//in mat4 view;
//in mat4 projection;

in vec3 position;
in vec2 textureUV;

uniform sampler2D texture1;

void main(){

	//color =  vec4(0.4, 0.0, 0.5, 1.0)+ (vec4(textureUV, 1.0, 1.0));
	color = texture(texture1, textureUV) * (vec4(textureUV, 1.0, 1.0));
	//color = texture(texture1, textureUV) +  vec4(0.0, 0.0, 0.0, 1.0);
};