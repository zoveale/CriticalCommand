#version 460

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitTangent;



out VS_OUT {
    vec3 FragPos;
    vec2 textureUV;
	vec3 normal;

    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;


	//Shadow stuff
	vec4 FragPosLightSpace;
} vs_out;

//out vec2 textureUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 PVM;

uniform mat4 lightSpaceMatrix;

void main(){
	vs_out.FragPos = vec3(model * vec4(position,1.0));
	vs_out.textureUV = aTexCoords; 

	vs_out.normal = mat3(transpose(inverse(model))) * aNormal;	

	vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0f);

	gl_Position = PVM * vec4(position, 1.0);
}