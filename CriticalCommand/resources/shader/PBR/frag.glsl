//PBR shading
#version 460 core
out vec4 FragColor;

in vec2 TexCoords;


uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D metalRoughAo;
uniform sampler2D gAlbedo;

// lights
const unsigned int maxShadowCastingPointLights = 10;
const unsigned int maxPointLights = 100;

uniform unsigned int numPointLights;
uniform unsigned int numShadowPointLights;
uniform vec3 pointLightPositions[maxPointLights];
uniform vec3 pointLightColors[maxPointLights];
uniform float radius[maxPointLights];
uniform samplerCube pointLightShadowCube[10];

uniform vec3 camPos;

const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
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
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}
// ----------------------------------------------------------------------------
float random (vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898,78.233)))* 43758.5453123);
}
// ---------------------------------------------------------------- ------------

void main(){	
	//uniform sampler2D gPosition;
	//uniform sampler2D gNormal;
	//uniform sampler2D metalRoughAo;
	//uniform sampler2D gAlbedo;
	vec3 albedo = texture(gAlbedo, TexCoords).rgb;
	vec3 posTexture = texture(gPosition, TexCoords).rgb;

	float metallic  = texture(metalRoughAo, TexCoords).r;
	float roughness = texture(metalRoughAo, TexCoords).g;
	float ao        = texture(metalRoughAo, TexCoords).b;

    vec3 N = texture(gNormal, TexCoords).rgb;

    vec3 V = normalize(camPos - posTexture);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow) 
	//TODO:: if no metallic, F0 = vec3(0.04f), if metallic, F0 = albedo
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

	 // ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).
    vec3 ambient = vec3(0.03) * albedo * ao;

    //  reflectance equation
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < numPointLights; ++i) 
    {
		float dis = length(pointLightPositions[i] - posTexture);

		if(dis < radius[i] * 4.0){

			float attenuation = 1.0 / (1.0 + ((4.5/radius[i]) * dis) + ((75.0/(radius[i]*radius[i])) * dis * dis)); 
			vec3 radiance = pointLightColors[i] * attenuation;
			// calculate per-light radiance
			vec3 L = normalize(pointLightPositions[i] - posTexture);
			vec3 H = normalize (V + L);

        

			// Cook-Torrance BRDF
			float NDF = DistributionGGX(N, H, roughness);   
			float G   = GeometrySmith(N, V, L, roughness);      
			vec3 F    = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);
           
			vec3 nominator    = NDF * G * F; 
			float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
			vec3 specular = nominator / max(denominator, 0.001); // prevent divide by zero for NdotV=0.0 or NdotL=0.0
        
			// kS is equal to Fresnel
			vec3 kS = F;
			// for energy conservation, the diffuse and specular light can't
			// be above 1.0 (unless the surface emits light); to preserve this
			// relationship the diffuse component (kD) should equal 1.0 - kS.
			vec3 kD = vec3(1.0) - kS;
			// multiply kD by the inverse metalness such that only non-metals 
			// have diffuse lighting, or a linear blend if partly metal (pure metals
			// have no diffuse light).
			kD *= 1.0 - metallic;	  

			// scale light by NdotL
			float NdotL = max(dot(N, L), 0.0);         

			// add to outgoing radiance Lo
			Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
			//Lo += vec3(0.03) * albedo * attenuation * ao;
		}
	}   
    
   

    vec3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 

    FragColor = vec4(color, 1.0);
}

// ----------------------------------------------------------------------------
//vec3 getNormalFromMap()
//{
//    vec3 tangentNormal = texture(material.texture_normal, TexCoords).xyz * 2.0 - 1.0;
//
//    vec3 Q1  = dFdx(WorldPos);
//    vec3 Q2  = dFdy(WorldPos);
//    vec2 st1 = dFdx(TexCoords);
//    vec2 st2 = dFdy(TexCoords);
//
//    vec3 N   = normalize(Normal);
//    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
//    vec3 B  = -normalize(cross(N, T));
//    mat3 TBN = mat3(T, B, N);
//
//    return normalize(TBN * tangentNormal);
//}
// ----------------------------------------------------------------------------