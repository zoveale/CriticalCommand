#version 460
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

struct Material{
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	sampler2D texture_normal1;
	sampler2D texture_height1;

	//sampler2D texture_emission1;

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

in VS_OUT {
    vec3 FragPos;
    vec2 textureUV;
	vec3 normal;
	//vec3 viewPos;

    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;

	//Shadow stuff
	//vec4 FragPosLightSpace;
} fs_in;

float Attenuation(vec3 pos, PointLight light);
float Attenuation(vec3 pos, SpotLight light);
float Intensity(float theta, SpotLight light);
vec3 CalcDirLight(DirLight light, Material material, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, Material material, vec3 fragPos, vec3 viewDir, float shadow); 
vec3 CalcSpotLight(SpotLight light, Material material, vec3 fragPos, vec3 viewDir);

//TODO: seperate shadowcasting and non shadow casting lights
vec3 CalcShadowCastingPointLight(PointLight light,
								 Material material,
								 vec3 fragPos,
								 vec3 viewDir,
								 float shadow); 

uniform Material material;
uniform DirLight dirLight;

//TODO::
uniform uint numPointLights;
#define NR_POINT_LIGHTS 10
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform uint numSpotLights;
#define NR_SPOT_LIGHTS 10
uniform SpotLight spotLights[NR_SPOT_LIGHTS];




//in vec2 textureUV;


//normal += texture(material.texture_normal1, fs_in.textureUV).rgb;
//vec3 norm = normalize(fs_in.normal);

vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
vec3 result = vec3(0.0);
//vec3 emission = texture(material.texture_emission1, fs_in.textureUV).rgb ; 

//test stuff
uniform float gamma;
uniform float heightScale;
//ShadowStuff
uniform float far_plane;
#define MAX_SHADOW_CASTING_POINT_LIGHTS 1
uniform samplerCube shadowCubeMap[1];
float near = 1.0f; 
float far  = 10.0f; 
float ShadowCalculationCubeMap(vec3 fragPos, vec3 lightPos, samplerCube shadowCube);
//uniform int pointLightNum;
float shadow = 0.5;
//ParallaxMapping Stuff
//vec2 ParallaxMapping(out vec2 texCoords, in vec3 viewDir, sampler2D height);

void main(){   

	for(uint i = 0; i < 1; i++){
		shadow = ShadowCalculationCubeMap(fs_in.FragPos, pointLights[i].position, shadowCubeMap[i]);
		result += CalcPointLight(pointLights[i], material, fs_in.TangentFragPos, viewDir, shadow); 
	}

	for(uint i = 0; i < numSpotLights; i++){
        result += CalcSpotLight(spotLights[i], material, fs_in.FragPos, viewDir); 
	} 
	
	FragColor =  vec4(result.rgb, 1.0f);
	float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));

    if(brightness > 0.85)
        BrightColor = vec4(FragColor.rgb, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);

	

	
}

float ShadowCalculationCubeMap(vec3 fragPos, vec3 lightPos, samplerCube shadowCube){
	vec3 fragToLight = fragPos - lightPos;
	float closestDepth = texture(shadowCube, fragToLight).r;
	closestDepth *= far_plane;
	float currentDepth = length(fragToLight);
	float bias = 0.5f;
	//float bias = max(0.5 * (1.0 - dot(norm, fragToLight)), 0.5);
    float shadow = currentDepth -  bias > closestDepth  ? 1.0 : 0.0;
	return shadow;
	}

//Parallax Mapping
void ParallaxMapping(in out vec2 texCoords, in vec3 viewDir, in sampler2D heightMap){
	
//	float height =  texture(heightMap, texCoords).r;    
//    vec2 p = viewDir.xy / viewDir.z * (height * heightScale);
//    texCoords = texCoords - p;   
	float height = 1- texture(heightMap, texCoords).r;    
    vec2 delta = viewDir.xy * height * heightScale ;
	texCoords = texCoords.xy - delta;
	}

float LinearizeDepth(float depth){
    float z = depth * 2.0 - 1.0; 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}
 
float Attenuation(vec3 pos, PointLight light){
		float dis = length(light.position - fs_in.FragPos);
		float attenuation = 1.0 / (light.constant + light.linear * dis
								 + light.quadratic * (dis * dis));   
		return attenuation;
	} 

float Attenuation(vec3 pos, SpotLight light){
		float dis = length(light.position - fs_in.FragPos);
		float attenuation = (1.0 / (1 + (light.linear * (dis * dis)))); 
		return attenuation;
	} 


vec3 CalcPointLight(PointLight light, Material material, vec3 fragPos, vec3 viewDir, float shadow){
	vec2 texCoords = fs_in.textureUV;

	//ParallaxMapping(texCoords, viewDir, material.texture_height1);

	vec3 nor = normalize(vec3(texture(material.texture_normal1, texCoords).rgb));
	vec3 norm = vec3(nor *2.0f - 1.0f);

    vec3 lightDir = normalize(fs_in.TangentLightPos - fragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
    float attenuation = Attenuation(fragPos, light);    
	
    vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse1, texCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.texture_diffuse1, texCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, texCoords));
	
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

	return (ambient + ((1.0 - shadow + 0.05f) * (diffuse + specular)));
    //return (ambient + diffuse + specular);// / (dot(viewPos, light.position));
	//return vec3(texture(material.texture_normal1, fs_in.textureUV).rgb);

} 

float Intensity(float theta, SpotLight light){
		float epsilon   = light.cutoff - light.outerCutoff;
		float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0); 
		return intensity;
	}

vec3 CalcSpotLight(SpotLight light, Material material, vec3 fragPos, vec3 viewDir){
	vec2 texCoords = fs_in.textureUV;

	//ParallaxMapping(texCoords, viewDir, material.texture_height1);

    vec3 lightDir = normalize(light.position - fragPos);

	//vec3 nor = normalize(vec3(texture(material.texture_normal1, texCoords).rgb));
	//vec3 norm = normalize(fs_in.normal); //vec3(nor *2.0f - 1.0f);
	vec3 nor = vec3(texture(material.texture_normal1, texCoords).rgb);
	vec3 norm = vec3(nor *2.0f - 1.0f);
	//fs_in.normal
    // diffuse shading
    float diff = max(dot(norm, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, norm);

	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
	
    float dis = length(light.position - fragPos);
    float attenuation = Attenuation(fragPos, light);   
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));

	//FIXME:: need to solve right scales between blender/dae and assimp/dae 
	//if(theta < light.cutoff) {return vec3(0.0);}

    float intensity = Intensity(theta, light);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, fs_in.textureUV));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1,fs_in.textureUV));
    vec3 specular = light.specular * spec;// * vec3(texture(material.texture_specular1, fs_in.textureUV));
	
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}


vec3 CalcDirLight(DirLight light, Material material,  vec3 normal, vec3 viewDir){
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse1, fs_in.textureUV));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.texture_diffuse1, fs_in.textureUV));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, fs_in.textureUV));
    
	return (ambient + diffuse + specular);
}  
