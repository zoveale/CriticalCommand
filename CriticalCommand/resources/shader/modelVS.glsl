#version 330 core
const int MAX_JOINTS = 50;
const int MAX_WEIGHTS = 3;


layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTextureCoords;
layout (location = 2) in vec3 inNormal;
layout (location = 4) in ivec3 inJointIndices;
layout (location = 5) in vec3 inWeights;


out vec2 textureCoords;
out vec3 normals;

uniform mat4 jointTransforms[MAX_JOINTS];
uniform mat4 projectionViewMatrix;

void main(){
	vec4 modelPos = vec4(0.0);
	vec4 totalNormal = vec4(0.0);

	for(int i = 0; i < MAX_WEIGHTS; i++){
	
	}
	gl_Position = projectionViewMatrix * modelPos;
	normals = totalNormal.xyz;
	textureCoords = inTextureCoords;
}
