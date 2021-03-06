//Fragment Deffered Shading
#version 460 core
//Out HDR and bloom from here?
//layout (location = 0) out vec4 FragColor;
//layout (location = 1) out vec4 BrightColor;
out vec4 FragColor;


in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

const int NR_LIGHTS = 100;
uniform uint numPointLights;

uniform PointLight pointLights[NR_LIGHTS];
uniform vec3 viewPos;


void main(){             
    // retrieve data from gbuffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;
    
    // then calculate lighting as usual
    vec3 lighting  = vec3(0.0f); // hard-coded ambient component
    vec3 viewDir  = normalize(viewPos - FragPos);
	 //numPointLights
    for(int i = 0; i < numPointLights; ++i){
        // diffuse
        vec3 lightDir = normalize(pointLights[i].position - FragPos);
        vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * pointLights[i].diffuse;
        // specular
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
        vec3 specular = pointLights[i].specular * spec * Specular * 1.1f;
        // attenuation
        float dis = length(pointLights[i].position - FragPos);
        float attenuation = 1.0 / (1.0 + 0.09 * dis + 0.032 * dis * dis);

        diffuse *= attenuation;
        specular *= attenuation * 0.1f;
        lighting += diffuse + specular;        
    }

    FragColor = vec4(lighting, 1.0);

//	float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
//    if(brightness > 0.85)
//        BrightColor = vec4(FragColor.rgb, 1.0);
//    else
//        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}