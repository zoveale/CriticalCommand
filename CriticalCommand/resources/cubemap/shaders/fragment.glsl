//skybox/cubemap
#version 460 core
layout (location = 0) out vec4 FragColor;
//layout (location = 1) out vec4 BrightColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main(){    
	//TODO:: Texcoords needs to be negative to load map correctly?
	vec3 envColor = texture(skybox, -TexCoords).rgb;
    
    envColor = envColor / (envColor + vec3(1.0));
    envColor = pow(envColor, vec3(1.0/2.2)); 
  
    FragColor = vec4(envColor, 1.0);
	
}