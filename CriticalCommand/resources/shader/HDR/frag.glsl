#version 460 core
out vec4 FragColor;
  
in vec2 textureUV;

uniform sampler2D hdrBuffer;
uniform sampler2D bloomBuffer;

const float gamma = 2.2;
float exposure = 1.0f;


void main(){             
		// reinhard
        //vec3 result = hdrColor / (hdrColor + vec3(1.0));
        // exposure
		vec3 hdrColor = texture(hdrBuffer, textureUV).rgb;
		vec3 bloomColor = texture(bloomBuffer, textureUV).rgb;

		hdrColor += bloomColor;
        vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
        // also gamma correct while we're at it       
        result = pow(result, vec3(1.0 / gamma));
		
        FragColor = vec4(result, 1.0);
		//FragColor = texture(hdrBuffer, textureUV);
		
		
}  