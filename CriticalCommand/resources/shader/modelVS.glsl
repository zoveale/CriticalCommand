#version 330 core
const int MAX_JOINTS = 50;
const int MAX_WEIGHTS = 3;


layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTextureCoords;
layout (location = 2) in vec3 inNormal;
//layout (location = 3) in vec3 tangent;??
//layout (location = 4) in vec3 bitTangent;??
layout (location = 5) in ivec3 inJointIndices;
layout (location = 6) in vec3 inWeights;


out vec2 textureCoords;
out vec3 normals;

uniform mat4 jointTransforms[MAX_JOINTS];
uniform mat4 projectionViewMatrix;

void main(){
	vec4 modelPos = vec4(0.0);
	vec4 totalNormal = vec4(0.0);

	for(int i = 0; i < MAX_WEIGHTS; i++){
		mat4 jointTransform = jointTransforms[inJointIndices[i]];
		vec4 posePosition = jointTransform * vec4(inPosition, 1.0);
		modelPos += posePosition * inWeights[i];

		vec4 worldNormal = jointTransform * vec4(inNormal, 0.0);
		totalNormal = worldNormal * inWeights[i];

	}
	gl_Position = projectionViewMatrix * modelPos;
	normals = totalNormal.xyz;
	textureCoords = inTextureCoords;
}
