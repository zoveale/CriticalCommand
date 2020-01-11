#version 460
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

uniform DirLight dirLight;

//TODO::
uniform uint numPointLights;
#define NR_POINT_LIGHTS 10
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform uint numSpotLights;
#define NR_SPOT_LIGHTS 10
uniform SpotLight spotLights[NR_SPOT_LIGHTS];


in VS_OUT {
    vec3 FragPos;
    vec2 textureUV;
	vec3 normal;

    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;

	//Shadow stuff
	vec4 FragPosLightSpace;
	
} fs_in;

//in vec2 textureUV;

uniform vec3 viewPos;

vec3 norm = normalize(fs_in.normal);
vec3 viewDir = normalize(viewPos - fs_in.FragPos);
vec3 result = vec3(0.0);
//vec3 emission = texture(material.texture_emission1, fs_in.textureUV).rgb ; 

//test stuff
uniform float gamma;

//ShadowStuff
uniform sampler2D shadowMap;
float near = 0.1; 
float far  = 1000.0; 
// required when using a perspective projection matrix
float LinearizeDepth(float depth);
float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir);

void main(){   
	for(uint i = 0; i < numPointLights; i++){
        result += CalcPointLight(pointLights[i], material, norm, fs_in.FragPos, viewDir); 
	}
	for(uint i = 0; i < numSpotLights; i++){
        result += CalcSpotLight(spotLights[i], material, norm, fs_in.FragPos, viewDir); 
	} 
	
	

	
	vec3 gammaCorrection = pow(result.rgb, vec3(1.0/gamma));
	FragColor =  vec4(vec3(gammaCorrection), 1.0);


	//float depth = LinearizeDepth(gl_FragCoord.z) / far;
	//FragColor = vec4(vec3(depth), 0.0 );
}

float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
	//float bias = 0.005;
	float bias = max(0.05 * (1.0 - dot(norm, lightDir)), 0.0025); 
    float shadow = currentDepth -  bias > closestDepth  ? 1.0 : 0.0;

    return shadow;
}

float LinearizeDepth(float depth){
    float z = depth * 2.0 - 1.0; 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}
 
float Attenuation(vec3 pos, PointLight light){
		float dis = length(light.position - pos);
		float attenuation = (1.0 / (1 + (light.linear * (dis * dis)))); 
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
    vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse1, fs_in.textureUV));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.texture_diffuse1, fs_in.textureUV));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, fs_in.textureUV));

    return (ambient + diffuse + specular);
}  


vec3 CalcPointLight(PointLight light, Material material, vec3 normal, vec3 fragPos, vec3 viewDir){
    vec3 lightDir = normalize(light.position - fragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);

	//TODO::blinn
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
	//TODO::phong
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	
    //attenuation
    float attenuation = Attenuation(fragPos, light);    
    // combine results
	float shadow = ShadowCalculation(fs_in.FragPosLightSpace, lightDir); 
	

    vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse1, fs_in.textureUV));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.texture_diffuse1, fs_in.textureUV));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, fs_in.textureUV));

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

	
	return (ambient + ((1.0 - shadow) * (diffuse + specular)));
    //return (ambient + diffuse + specular);// / (dot(viewPos, light.position));
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

	//TODO::blinn
	vec3 halfwayDir = normalize(lightDir+ viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
	//TODO::phong
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));

	//FIXME:: need to solve right scales between blender/dae and assimp/dae 
	//if(theta < light.cutoff) {return vec3(0.0);}

    float intensity = Intensity(theta, light);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, fs_in.textureUV));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1,fs_in.textureUV));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, fs_in.textureUV));
	
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}