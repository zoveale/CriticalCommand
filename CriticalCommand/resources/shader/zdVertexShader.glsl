#version 330 core

layout(location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normals;

//
out vec2 textureUV;
out vec3 normal;
out vec3 FragPos;
//
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main(){
	
	FragPos = vec3(model * vec4(position, 1.0));
	textureUV = uv;

	//normal = normals;
	normal = mat3(transpose(inverse(model))) * normals;
	gl_Position =  projection * view  *  model *  vec4(position, 1.0f);
};
