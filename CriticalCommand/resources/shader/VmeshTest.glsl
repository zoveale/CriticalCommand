#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitTangent;
layout (location = 5) in ivec4 inBoneIDs;
layout (location = 6) in vec4 inWeights;

out vec2 TexCoords;
out vec4 weights;
out ivec4 jointIndex;
//normals

const int MAX_BONES = 100;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 gBones[MAX_BONES];


void main(){

	mat4 BoneTransform = gBones[inBoneIDs[0]] * inWeights[0];
    BoneTransform += gBones[inBoneIDs[1]] * inWeights[1];
    BoneTransform += gBones[inBoneIDs[2]] * inWeights[2];
    BoneTransform += gBones[inBoneIDs[3]] * inWeights[3];
	vec4 position = BoneTransform * vec4(aPos, 1.0);
	weights = inWeights;
    TexCoords = aTexCoords;    
    gl_Position = projection * view * model * position;
}