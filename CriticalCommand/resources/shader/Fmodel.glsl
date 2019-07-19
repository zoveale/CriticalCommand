#version 330 core
out vec4 FragColor;

struct Material{
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	sampler2D texture_normal1;
	sampler2D texture_height1;
	sampler2D texture_emission1;

	float shininess;
};
struct DirLight{
	vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutoff;
    float outerCutoff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

float Attenuation(vec3 pos, PointLight l);
float Intensity(float theta, SpotLight light);
vec3 CalcDirLight(DirLight light, Material material, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, Material material,vec3 normal, vec3 fragPos, vec3 viewDir); 
vec3 CalcSpotLight(SpotLight light, Material material, vec3 normal, vec3 fragPos, vec3 viewDir);

uniform Material material;
uniform SpotLight spotLight;
uniform DirLight dirLight;

#define NR_POINT_LIGHTS 4 
uniform PointLight pointLights[NR_POINT_LIGHTS];


in VS_OUT {
    vec3 FragPos;
    
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;

	vec3 normal;
} fs_in;

in vec2 textureUV;

uniform vec3 viewPos;

vec3 norm = normalize(fs_in.normal);
vec3 viewDir = normalize(viewPos - fs_in.FragPos);
vec3 result = vec3(0.0);
//vec3 emission = texture(material.texture_emission1, textureUV).rgb ; 

void main(){   
	for(int i = 0; i < 1; i++){
        result += CalcPointLight(pointLights[i], material, norm, fs_in.FragPos, viewDir); 
		}
	FragColor = vec4(result, 1.0);
}

float Attenuation(vec3 pos, PointLight light){
		float dis = length(light.position - pos);
		float attenuation = (1.0 / (light.constant + (light.linear * dis) + (light.quadratic * (dis * dis)))); 
		return attenuation;
	}

vec3 CalcDirLight(DirLight light, Material material,  vec3 normal, vec3 viewDir){
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse1, textureUV));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.texture_diffuse1, textureUV));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, textureUV));

    return (ambient + diffuse + specular);
}  


vec3 CalcPointLight(PointLight light, Material material, vec3 normal, vec3 fragPos, vec3 viewDir){
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    
    float attenuation = Attenuation(fragPos, light);    
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse1, textureUV));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.texture_diffuse1, textureUV));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, textureUV));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 

float Intensity(float theta, SpotLight light){
		float epsilon   = light.cutoff - light.outerCutoff;
		float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0); 
		return intensity;
	}

vec3 CalcSpotLight(SpotLight light, Material material, vec3 normal, vec3 fragPos, vec3 viewDir){
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float intensity = Intensity(theta, light);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, textureUV));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1,textureUV));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, textureUV));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}