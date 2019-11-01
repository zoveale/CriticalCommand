#version 460 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
	vec3 FragPos;
    
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;

	vec3 normal;
} gs_in;

out GS_OUT {
	vec3 FragPos;
    
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;

	vec3 normal;
} gs_out;
const float MAGNITUDE = 0.4;

void GenerateLine(int index)
{
//    gl_Position = gl_in[index].gl_Position;
//    EmitVertex();
//    gl_Position = gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * MAGNITUDE;
//    EmitVertex();
//    EndPrimitive();
}

void main(){

} 