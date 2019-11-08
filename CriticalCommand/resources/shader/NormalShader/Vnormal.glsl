#version 460 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitTangent;
out VS_OUT {
    vec3 normal;
	vec3 tangent;
	vec3 bitTangent;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    mat3 normalMatrix = mat3(transpose(inverse(view * model)));
    vs_out.normal = vec3(projection * vec4(normalMatrix * aNormal, 0.0));
	vs_out.tangent = vec3(projection * vec4(normalMatrix * tangent, 0.0));
	vs_out.bitTangent = vec3(projection * vec4(normalMatrix * bitTangent, 0.0));
    gl_Position = projection * view * model * vec4(position, 1.0); 
}