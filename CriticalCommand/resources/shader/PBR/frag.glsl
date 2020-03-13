//PBR shading
#version 460 core
out vec4 FragColor;

in vec2 TexCoords;


uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D metalRoughAo;
uniform sampler2D gAlbedo;

uniform samplerCube irradianceMap;

// lights
const unsigned int MAX_SHADOW_CASTING_POINT_LIGHTS = 13;
const unsigned int MAX_POINT_LIGHTS = 100;

const unsigned int MAX_SHADOW_CASTING_SPOT_LIGHTS = 1;
const unsigned int MAX_SPOT_LIGHTS = 100;

	
struct Material{
	vec3 albedo;
	vec3 fragmentPosition;
	float metallic ;
	float roughness;
	float ao;    
}material;

struct SpotLight{
	vec3 position;
	vec3 direction;
	vec3 color;

	float radius;
	float cutoff;
	float outerCutoff;

	float constant;
    float linear;
    float quadratic;
};

struct ShadowCastingSpotLight{
	vec3 position;
	vec3 color;
	float radius;
	float cutoff;
	float outerCutoff;
};

struct PointLight{
	vec3 position;
	vec3 color;
	float radius;
};

struct ShadowCastingPointLight{
	vec3 position;
	vec3 color;
	float radius;
	samplerCube shadowMap;
};

uniform unsigned int numPointLights;
uniform unsigned int numShadowPointLights;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform ShadowCastingPointLight shadowCastingPointLights[MAX_SHADOW_CASTING_POINT_LIGHTS];


uniform unsigned int numSpotLights;
uniform unsigned int numShadowSpotLights;
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];
uniform ShadowCastingSpotLight shadowCastingSpotLights[MAX_SHADOW_CASTING_SPOT_LIGHTS];

uniform vec3 camPos;


// ----------------------------------------------------------------------------
const float PI = 3.14159265359;
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness);
float random (vec2 st);
// ----------------------------------------------------------------------------
float ShadowCalculationCubeMap(vec3 fragPos, vec3 lightPos, samplerCube shadowCube);
// ----------------------------------------------------------------------------
void ShadowCastPointLightCalculator(in ShadowCastingPointLight light[MAX_SHADOW_CASTING_POINT_LIGHTS],
									in Material mat, in vec3 normal, in vec3 view, in vec3 fzero, in out vec3 reflectance);
// ----------------------------------------------------------------------------
void PointLightCalculator(in PointLight light[MAX_POINT_LIGHTS], in Material mat, in vec3 normal,
									in vec3 view, in vec3 fzero, in out vec3 reflectance);
// ----------------------------------------------------------------------------
void SpotLightCalculator(in SpotLight light[MAX_SPOT_LIGHTS], in Material mat, in vec3 normal, 
									in vec3 view, in vec3 fzero, in out vec3 reflectance);
// ----------------------------------------------------------------------------

void main(){	
	material.albedo = texture(gAlbedo, TexCoords).rgb;
	material.fragmentPosition = texture(gPosition, TexCoords).rgb;
	material.metallic  = texture(metalRoughAo, TexCoords).r;
	material.roughness = texture(metalRoughAo, TexCoords).g;
	material.ao        = texture(metalRoughAo, TexCoords).b;

    vec3 N = texture(gNormal, TexCoords).rgb;;
    vec3 V = normalize(camPos - material.fragmentPosition.rgb);

	//surface reflection at zero incidence
    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow) 
	//TODO:: if no metallic, F0 = vec3(0.04f), if metallic, F0 = albedo
	//vec3 F0 = (material.metallic >= 0.04f) ?   vec3(0.04f) : material.albedo ;
    vec3 F0 = material.albedo; 
    F0 = mix(F0, material.albedo, material.metallic);

	 // ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).
	
    //vec3 ambient = irradiance * material.albedo * material.ao;
	//vec3 ambient = vec3(0.0);

    //  reflectance equation
    vec3 Lo = vec3(0.0);

	//ShadowCastPointLightCalculator(shadowCastingPointLights, material, N, V, F0, Lo);
	//SpotLightCalculator(spotLights, material, N, V, F0, Lo);
	PointLightCalculator(pointLights, material, N, V, F0, Lo);
    
	vec3 irradiance = texture(irradianceMap, -N).rgb;
	vec3 kS = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, material.roughness);
    vec3 kD = kS;
	//kD *= 1.0 - material.metallic;	
	//TODO:: metallic textures are in the wrong format? cant subtract by 1.0
	kD *= material.metallic;	
    vec3 diffuse = irradiance * material.albedo;
    vec3 ambient = (kD * diffuse) *  material.ao;


    vec3 color =  ambient + Lo;
	//color = color * diffuse;
    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
	//I like the way it looks when gamma correct commented out
    color = pow(color, vec3(1.0/2.2)); 

    FragColor = vec4(color, 1.0);
}

void ShadowCastPointLightCalculator(in ShadowCastingPointLight light[MAX_SHADOW_CASTING_POINT_LIGHTS],
									in Material mat, in vec3 normal,
									in vec3 view, in vec3 fzero,
									in out vec3 reflectance){
	for(int i = 0; i < numShadowPointLights; ++i){
		float dis = length(light[i].position - mat.fragmentPosition);
		float visibility  = ShadowCalculationCubeMap(mat.fragmentPosition, light[i].position, light[i].shadowMap);

		float attenuation = 1.0 / (1.0 + ((4.5 / light[i].radius) * dis) + 
			((75.0/(light[i].radius * light[i].radius)) * dis * dis)); 
			
		vec3 radiance = light[i].color * attenuation;
		// calculate per-light radiance
		vec3 L = normalize(light[i].position - mat.fragmentPosition);
		vec3 H = normalize (view + L);

		// Cook-Torrance BRDF
		float NDF = DistributionGGX(normal, H, mat.roughness);   
		float G   = GeometrySmith(normal, view, L, mat.roughness);      
		vec3 F    = fresnelSchlick(clamp(dot(H, view), 0.0, 1.0), fzero);
           
		vec3 nominator    = NDF * G * F; 
		float denominator = 4 * max(dot(normal, view), 0.0) * max(dot(normal, L), 0.0);
		vec3 specular = nominator / max(denominator, 0.001); // prevent divide by zero for NdotV=0.0 or NdotL=0.0
		//(1.0 - visibility)
		// kS is equal to Fresnel
		vec3 kS = F;
		// for energy conservation, the diffuse and specular light can't
		// be above 1.0 (unless the surface emits light); to preserve this
		// relationship the diffuse component (kD) should equal 1.0 - kS.
		vec3 kD = vec3(1.0) - kS;
		// multiply kD by the inverse metalness such that only non-metals 
		// have diffuse lighting, or a linear blend if partly metal (pure metals
		// have no diffuse light).
		kD *= 1.0 - mat.metallic;	  
			
		// scale light by NdotL
		float NdotL = max(dot(normal, L), 0.0);         

		// add to outgoing radiance Lo
		// note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
		reflectance += (kD * mat.albedo / (PI + specular * (1.0 - visibility)) * radiance * (1.0 - visibility) * NdotL);  
	}  

}

void PointLightCalculator(in PointLight light[MAX_POINT_LIGHTS],
									in Material mat, in vec3 normal,
									in vec3 view, in vec3 fzero,
									in out vec3 reflectance){
	for(int i = 0; i < numPointLights; ++i){

		float dis = length(light[i].position - mat.fragmentPosition);
		//float attenuation = 1.0 / (dis * dis);

		float attenuation = 1.0 / (1.0 + ((4.5 / light[i].radius) * dis) + 
			((75.0/(light[i].radius * light[i].radius)) * dis * dis)); 

		vec3 radiance = light[i].color * attenuation;
		// calculate per-light radiance
		vec3 L = normalize(light[i].position - mat.fragmentPosition);
		vec3 H = normalize(view + L);

		// Cook-Torrance BRDF
		float NDF = DistributionGGX(normal, H, mat.roughness);   
		float G   = GeometrySmith(normal, view, L, mat.roughness);      
		vec3 F    = fresnelSchlick(clamp(dot(H, view), 0.0, 1.0), fzero);
           
		vec3 nominator    = NDF * G * F; 
		float denominator = 4 * max(dot(normal, view), 0.0) * max(dot(normal, L), 0.0);
		vec3 specular = nominator / max(denominator, 0.001); // prevent divide by zero for NdotV=0.0 or NdotL=0.0
		//(1.0 - visibility)
		// kS is equal to Fresnel
		vec3 kS = F;
		// for energy conservation, the diffuse and specular light can't
		// be above 1.0 (unless the surface emits light); to preserve this
		// relationship the diffuse component (kD) should equal 1.0 - kS.
		vec3 kD = vec3(1.0) - kS;
		// multiply kD by the inverse metalness such that only non-metals 
		// have diffuse lighting, or a linear blend if partly metal (pure metals
		// have no diffuse light).
		kD *= 1.0 - mat.metallic;	  
			
		// scale light by NdotL
		float NdotL = max(dot(normal, L), 0.0);         

		// add to outgoing radiance Lo
		// note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
		(reflectance += ((kD * mat.albedo  / PI + specular) * radiance * NdotL));  
	}  
}

void SpotLightCalculator(in SpotLight light[MAX_SPOT_LIGHTS],
									in Material mat, in vec3 normal,
									in vec3 view, in vec3 fzero,
									in out vec3 reflectance){

	for(int i = 0; i < numSpotLights; ++i){

		float dis = length(light[i].position - mat.fragmentPosition);

		float attenuation = 1.0 / (light[i].constant + light[i].linear * dis + light[i].quadratic * (dis * dis));
		//float attenuation = 1.0 / (dis * dis);
			
		vec3 radiance = light[i].color * attenuation;
		// calculate per-light radiance
		vec3 L = normalize(light[i].position - mat.fragmentPosition);
		vec3 H = normalize (view + L);

		float theta = dot(L, normalize(-light[i].direction)); 
		float epsilon = (light[i].cutoff - light[i].outerCutoff);
		float intensity = clamp((theta - light[i].outerCutoff) / epsilon, 0.0, 1.0);

		// Cook-Torrance BRDF
		float NDF = DistributionGGX(normal, H, mat.roughness);   
		float G   = GeometrySmith(normal, view, L, mat.roughness);      
		vec3 F    = fresnelSchlick(clamp(dot(H, view), 0.0, 1.0), fzero);
           
		vec3 nominator    = NDF * G * F; 
		float denominator = 4 * max(dot(normal, view), 0.0) * max(dot(normal, L), 0.0);
		vec3 specular = nominator / max(denominator, 0.001); // prevent divide by zero for NdotV=0.0 or NdotL=0.0
		//specular *= intensity;
		
		// kS is equal to Fresnel
		vec3 kS = F ;// * intensity;
		// for energy conservation, the diffuse and specular light can't
		// be above 1.0 (unless the surface emits light); to preserve this
		// relationship the diffuse component (kD) should equal 1.0 - kS.
		vec3 kD = vec3(1.0) - kS;
		// multiply kD by the inverse metalness such that only non-metals 
		// have diffuse lighting, or a linear blend if partly metal (pure metals
		// have no diffuse light).
		kD *= 1.0 - mat.metallic;	  
			
		// scale light by NdotL
		float NdotL = max(dot(normal, L), 0.0);   
		
		// add to outgoing radiance Lo
		// note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
		reflectance += (kD * mat.albedo / (PI + specular * intensity) * radiance * intensity * NdotL);  
	}  
}
float DistributionGGX(vec3 N, vec3 H, float roughness){
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
    return nom / max(denom, 0.001); // prevent divide by zero for roughness=0.0 and NdotH=1.0
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness){
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;
    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness){
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0){
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness){
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}
// ----------------------------------------------------------------------------
float random (vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898,78.233)))* 43758.5453123);
}
// ----------------------------------------------------------------------------
float ShadowCalculationCubeMap(vec3 fragPos, vec3 lightPos, samplerCube shadowCube){
    vec3 fragToLight = fragPos - lightPos;
    float closestDepth = texture(shadowCube, fragToLight).r;
    closestDepth *= 55.0;
    float currentDepth = length(fragToLight);
    float bias = 0.5; 
    float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;
	return shadow;
}

