#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitTangent;



out VS_OUT {
    vec3 FragPos;
    
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;

	vec3 normal;
} vs_out;

out vec2 textureUV;

//normals
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 PVM;

void main(){
	vs_out.FragPos = vec3(model * vec4(position,1.0));
	textureUV = aTexCoords; 

	vs_out.normal = mat3(transpose(inverse(model))) * aNormal;	

	gl_Position = PVM * vec4(position, 1.0);
}