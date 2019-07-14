#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitTangent;

out vec2 TexCoords;
//out vec4 weights; 
out vec3 normals;

//normals
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 PVM;

vec4 BoneTransform = vec4(0.0);
void main(){
		normals = mat3(transpose(inverse(model))) * aNormal;	
		TexCoords = aTexCoords;    
		gl_Position = PVM * vec4(aPos, 1.0);
}