//Vertex Deffered Shading
#version 460 core
layout (location = 0) in vec3 aPos;
//layout (location = NULL) in vec3 aNormal;
layout (location = 1) in vec2 aTexCoords;
//layout (location = 3) in vec3 aTangent;
//layout (location = 4) in vec3 aBitTangent;

out vec2 TexCoords;

void main(){
	TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 1.0);
}    