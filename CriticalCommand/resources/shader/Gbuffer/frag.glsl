//Gbuffer
#version 460 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 metalRoughAo;
layout (location = 3) out vec4 gAlbedo;


in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

struct Material{
	sampler2D texture_albedo;
	sampler2D texture_normal;
	sampler2D texture_metallic;
	sampler2D texture_roughness;
	sampler2D texture_ao;
};

uniform Material material;

vec3 getNormalFromMap(){
    vec3 tangentNormal = texture(material.texture_normal, TexCoords).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(FragPos);
    vec3 Q2  = dFdy(FragPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}



void main(){             
	// store the fragment position vector in the first gbuffer texture
    gPosition = FragPos;
    // also store the per-fragment normals into the gbuffer
	//TODO::swap to texture_normal1;
    //gNormal = worldSpaceinverse * texture(material.texture_normal1, TexCoords).rgb;
	//gNormal = normalize(texture(material.texture_normal1, TexCoords).rgb);
	//gNormal = gNormal * 2.0f - 1.0f;
	gNormal = getNormalFromMap();
	
    // and the diffuse per-fragment color
    metalRoughAo.r = texture(material.texture_metallic, TexCoords).r;
	metalRoughAo.g = texture(material.texture_roughness, TexCoords).r;
	metalRoughAo.b = texture(material.texture_ao, TexCoords).r;

	gAlbedo.rgb = texture(material.texture_albedo, TexCoords).rgb;
	gAlbedo.rgb = vec3(pow(gAlbedo.r, 2.2f), pow(gAlbedo.g, 2.2f), pow(gAlbedo.b, 2.2f));
	gAlbedo.a = 1.0f;

    // store specular intensity in gAlbedoSpec's alpha component
    // gAlbedoSpec.a = normalize(texture(material.texture_specular1, TexCoords).r);
}  