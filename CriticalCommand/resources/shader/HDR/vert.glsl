#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 0) in vec2 textureCoords;

out vec2 textureUV;
uniform mat4 model;

void main(){
	textureUV = textureCoords;
    gl_Position = model * vec4(aPos, 1.0);
}    