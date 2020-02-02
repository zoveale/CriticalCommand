//Gbuffer
#version 460 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
  
in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

struct Material{
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	sampler2D texture_normal1;
	sampler2D texture_height1;

	//sampler2D texture_emission1;

	//float shininess;
};

uniform Material material;

void main(){             
	// store the fragment position vector in the first gbuffer texture
    gPosition = FragPos;
    // also store the per-fragment normals into the gbuffer
	//TODO::swap to texture_normal1;
    //gNormal = worldSpaceinverse * texture(material.texture_normal1, TexCoords).rgb;
	//gNormal = normalize(texture(material.texture_normal1, TexCoords).rgb);
	//gNormal = gNormal * 2.0f - 1.0f;
	gNormal = normalize(Normal);
	
    // and the diffuse per-fragment color
    gAlbedoSpec.rgb = texture(material.texture_diffuse1, TexCoords).rgb;
    // store specular intensity in gAlbedoSpec's alpha component
    gAlbedoSpec.a = normalize(texture(material.texture_specular1, TexCoords).r);
}  