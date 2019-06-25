#version 330 core
out vec4 color;

struct Material{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct Light{
	vec3 position;
	vec3 direction;
	float cutoff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
    float linear;
    float quadratic;

	
	//float attenuation;
};
float Attenuation(vec3 pos, Light l);

uniform Material material;
uniform Light light;



//in vec3 position;
in vec2 textureUV;
in vec3 normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;


uniform sampler2D texture1;

//float ambientStrength = 0.1;
//float specularStrength = 0.9;

vec3 norm = normalize(normal);
vec3 lightDir = normalize(light.position - FragPos); 
float theta = dot(lightDir, normalize(-light.direction));
//vec3 lightDir = normalize(-light.direction); 
float diff = max(dot(norm, lightDir), 0.0);



vec3 viewDir = normalize(viewPos - FragPos);
vec3 reflectDir = reflect(-lightDir, norm); 
float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

//vec3 ambient = light.ambient * vec3(texture(material.diffuse, textureUV));
vec3 ambient = light.ambient * texture(material.diffuse, textureUV).rgb;
vec3 diffuse = light.diffuse * diff * texture(material.diffuse, textureUV).rgb;
vec3 specular = light.specular * spec * texture(material.specular, textureUV).rgb;    
vec3 emission = texture(material.emission, textureUV).rgb ; 

float attenuation = Attenuation(FragPos, light);


void main(){

	if(theta > light.cutoff){  
		//ambient  *= attenuation;
		diffuse  *= attenuation;
		specular *= attenuation;

		vec3 result = ambient + diffuse + specular + (emission * 0.3f);
		color = vec4(result, 1.0);
    }
    else{
		color = vec4(light.ambient * texture(material.diffuse, textureUV).rgb, 1.0);
	}

	
  
	
	
	
	
	//color = texture(texture1, textureUV) * vec4(result, 1.0f);


	//color = texture(texture1, textureUV) * vec4(lightColor*objectColor, 1.0f);
	
	//color =  (vec4(0.4, 0.0, 0.5, 1.0)* (vec4(textureUV, 1.0, 1.0)));
	//color = texture(texture1, textureUV);// * vec4(lightColor*objectColor,1.0);
	//color = texture(texture1, textureUV) +  vec4(0.0, 0.0, 0.0, 1.0);
};

float Attenuation(vec3 pos, Light l){
		float attenuation;
		float distance = length(l.position - pos);
		attenuation = (1.0 / (l.constant + (l.linear * distance) + (l.quadratic * (distance * distance)))); 
		return attenuation;
	}