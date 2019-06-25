#version 330 core

out vec4 color;

in vec3 position;
in vec2 textureUV;
in vec3 normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;


uniform sampler2D texture1;

float ambientStrength = 0.1;
vec3 ambient = ambientStrength * lightColor;

vec3 norm = normalize(normal);
vec3 lightDir = normalize(lightPos - FragPos); 
float diff = max(dot(norm, lightDir), 0.0);
vec3 diffuse = diff * lightColor;

float specularStrength = 0.9;
vec3 viewDir = normalize(viewPos - FragPos);
vec3 reflectDir = reflect(-lightDir, norm); 
float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
vec3 specular = specularStrength * spec * lightColor;  




void main(){

    
	vec3 result = (ambient + diffuse + specular) * objectColor;

	color = texture(texture1, textureUV) * vec4(result, 1.0f);


	//color = texture(texture1, textureUV) * vec4(lightColor*objectColor, 1.0f);
	
	//color =  (vec4(0.4, 0.0, 0.5, 1.0)* (vec4(textureUV, 1.0, 1.0)));
	//color = texture(texture1, textureUV);// * vec4(lightColor*objectColor,1.0);
	//color = texture(texture1, textureUV) +  vec4(0.0, 0.0, 0.0, 1.0);
};