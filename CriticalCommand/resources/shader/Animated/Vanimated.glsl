#version 460
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitTangent;
layout (location = 5) in ivec4 inBoneIDs;
layout (location = 6) in vec4 inWeights;

out vec2 textureUV;
out vec3 FragPos;
out vec3 normal;

const int MAX_WEIGHTS = 4;
const int MAX_BONES = 100;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform mat4 gBones[MAX_BONES];
uniform mat4 PVM;

vec4 BoneTransform = vec4(0.0);
void main(){

		for(int i = 0; i < MAX_WEIGHTS; i++){
			vec4 localPos = gBones[inBoneIDs[i]] * vec4(aPos, 1.0);
			BoneTransform += localPos * inWeights[i];
		}
	
		FragPos = vec3(model* BoneTransform);
		textureUV = aTexCoords;    
		gl_Position = projection * view * model * BoneTransform;//vec4(aPos, 1.0);//
	}


/*
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
*/